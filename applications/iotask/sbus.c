#include "iotask/sbus.h"


/* 遥控器按键全局数据 */
sbus_t sbus = {
    .ch1  = SBUS_CH_OFFSET,
    .ch2  = SBUS_CH_OFFSET,
    .ch3  = SBUS_CH_OFFSET,
    .ch4  = SBUS_CH_OFFSET,
    .ch5  = SBUS_CH_OFFSET,
    .ch6  = SBUS_CH_OFFSET,
    .ch7  = SBUS_CH_OFFSET,
    .ch8  = SBUS_CH_OFFSET,
    .ch9  = SBUS_CH_OFFSET,
    .ch10 = SBUS_CH_OFFSET,
    .ch11 = SBUS_CH_OFFSET,
    .ch12 = SBUS_CH_OFFSET,
    .ch13 = SBUS_CH_OFFSET,
    .ch14 = SBUS_CH_OFFSET,
    .ch15 = SBUS_CH_OFFSET,
    .ch16 = SBUS_CH_OFFSET,
};

/*--------------------------  控制块  ---------------------------*/

/* 串口设备句柄 */
static rt_device_t serial;

/* 串口接收消息结构*/
struct rx_msg
{
    rt_device_t dev;
    rt_size_t size;
};


/* 消息队列控制块 */
static struct rt_messagequeue rx_mq;


/*--------------------------  DMA中断回调  ---------------------------*/

/* 接收数据回调函数 */
static rt_err_t sbus_uart_input(rt_device_t dev, rt_size_t size)
{
    struct rx_msg msg;
    rt_err_t result;
    msg.dev = dev;
    msg.size = size;

    result = rt_mq_send(&rx_mq, &msg, sizeof(msg));
    if ( result == -RT_EFULL)
    {
        /* 消息队列满 */
        rt_kprintf("message queue full！\n");
    }
    return result;
}

/*--------------------------  数据处理线程  ---------------------------*/

static void sbus_thread_entry(void *parameter)
{
    struct rx_msg msg;
    rt_err_t result;
    rt_uint32_t rx_length;
	
    rt_uint8_t rx_buffer[RT_SERIAL_RB_BUFSZ + 1];
    rt_uint8_t* byte = rx_buffer;                   /* 缓冲区别名 */
    
    rt_uint8_t data_count = 0;
    
    while (1)
    {
        rt_memset(&msg, 0, sizeof(msg));
        
        /* 从消息队列中读取消息*/
        result = rt_mq_recv(&rx_mq, &msg, sizeof(msg), RT_WAITING_FOREVER);
        if (result == RT_EOK)
        {
            /* 从串口读取数据*/
            rx_length = rt_device_read(msg.dev, 0, &rx_buffer[data_count], msg.size);
            data_count = data_count + rx_length;
			
//			rt_kprintf("%d\n",rx_length);
            
            /* 可以使用状态机解包 */
            if (byte[0] == 0x0F)
            {
                /* 满足帧头才判断帧长 */
                if (data_count >= 25) 
                {
                    sbus.ch1  = (byte[1] >> 0 | byte[2] << 8) & 0x7FF;
                    sbus.ch2  = (byte[2] >> 3 | byte[3] << 5) & 0x7FF;
                    sbus.ch3  = (byte[3] >> 6 | byte[4] << 2 | byte[5] << 10) & 0x7FF;
                    sbus.ch4  = (byte[5] >> 1 | byte[6] << 7) & 0x7FF;
                    sbus.ch5  = (byte[6] >> 4 | byte[7] << 4) & 0x7FF;
                    sbus.ch6  = (byte[7] >> 7 | byte[8] << 1 | byte[9] << 9) & 0x7FF;
                    sbus.ch7  = (byte[9] >> 2 | byte[10] << 6) & 0x7FF;
                    sbus.ch8  = (byte[10] >>5 | byte[11] << 3) & 0x7FF;
                    sbus.ch9  = (byte[12] >>0 | byte[13] << 8) & 0x7FF;
                    sbus.ch10 = (byte[13] >> 3 | byte[14] << 5) & 0x7FF;
                    sbus.ch11 = (byte[14] >> 6 | (byte[15] << 2 ) | byte[16] << 10 ) & 0x07FF;
                    sbus.ch12 = (byte[16] >> 1 | (byte[17] << 7 )) & 0x07FF;
                    sbus.ch13 = (byte[17] >> 4 | (byte[18] << 4 )) & 0x07FF;
                    sbus.ch14 = (byte[18] >> 7 | (byte[19] << 1 ) | byte[20] << 9 ) & 0x07FF;
                    sbus.ch15 = (byte[20] >> 2 | (byte[21] << 6 )) & 0x07FF;
                    sbus.ch16 = (byte[21] >> 5 | (byte[22] << 3 )) & 0x07FF;

                    /* 准备下一数据帧接收 */
                    data_count = 0;                
                }
            }
            else /*帧头不对，重新接收等待帧头*/
            {
                data_count = 0;
            }
        }
    }
}

int sbus_init(void)
{
    rt_err_t ret = RT_EOK;
    static char msg_pool[256];
    
    /* sbus串口的初始化配置参数 */
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  
    config.baud_rate = 100000;
    config.data_bits = DATA_BITS_8;
    config.parity    = PARITY_EVEN;
    config.stop_bits = STOP_BITS_2;
    
    /* 查找串口设备 */
    serial = rt_device_find(SBUS_UART);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", SBUS_UART);
        return RT_ERROR;
    }

    /* 初始化消息队列 */
    rt_mq_init(&rx_mq, "mq_sbus",
               msg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),    /* 一条消息的最大长度 */
               sizeof(msg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);      
    
    /* 以 DMA 接收及轮询发送方式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_DMA_RX);
    
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, sbus_uart_input);

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("sbus", 
                                        sbus_thread_entry, 
                                        RT_NULL, 
                                        SBUS_THREAD_STACK_SIZE, 
                                        SBUS_THREAD_PRIORITY, SBUS_THREAD_TIMESLICE);
    
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
    else
    {
        ret = RT_ERROR;
    }

    return ret;
}

/* 导出命令 or 自动初始化 */
INIT_APP_EXPORT(sbus_init);

/*--------------------------  调试输出线程  ---------------------------*/

static void sbus_debug_thread_entry(void *parameter)
{
    while (1)
    {
        rt_kprintf("\x1b[2J\x1b[H");
        rt_kprintf("%4d %4d\n", sbus.ch3, sbus.ch1);
        rt_kprintf("%4d %4d\n", sbus.ch4, sbus.ch2);
        rt_kprintf("%4d %4d %4d %4d\n", sbus.ch5, sbus.ch6, sbus.ch7, sbus.ch8);
        rt_kprintf("%4d %4d %4d %4d\n", sbus.ch9, sbus.ch10, sbus.ch11, sbus.ch12);
        rt_kprintf("%4d %4d %4d %4d\n", sbus.ch13, sbus.ch14, sbus.ch15, sbus.ch16);
        rt_thread_mdelay(10);
    }
}

static int sbus_output(int argc, char *argv[])
{
    rt_thread_t thread = rt_thread_create("sbus_output", sbus_debug_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }

    return 0;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(sbus_output, remote controller data debug output);


