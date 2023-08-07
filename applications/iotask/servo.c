#include "iotask/servo.h"


 /* 用于接收消息的信号量 */
static struct rt_semaphore rx_sem;
static rt_device_t serial;

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size)
{
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(&rx_sem);

    return RT_EOK;
}

static void serial_thread_entry(void *parameter)
{
    char ch;

    while (1)
    {
        /* 从串口读取一个字节的数据，没有读取到则等待接收信号量 */
        while (rt_device_read(serial, -1, &ch, 1) != 1)
        {
            /* 阻塞等待接收信号量，等到信号量后再次读取数据 */
            rt_sem_take(&rx_sem, RT_WAITING_FOREVER);
        }
        /* 读取到的数据通过串口错位输出 */
//        ch = ch + 1;
//        rt_device_write(serial, 0, &ch, 1);
    }
}


static void serial_send_thread_entry(void *parameter)
{
	rt_thread_mdelay(500);
	
	rt_uint8_t send_data[7] = {0x55, 0x55, 0x05, 0x06, 0x64, 0x00, 0x00};
	
    /* 发送字符串 */
    rt_device_write(serial, 0, send_data, 7);

}

static int uart_sample(void)
{
    rt_err_t ret = RT_EOK;

	struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;  
    config.baud_rate = BAUD_RATE_9600;
    config.data_bits = DATA_BITS_8;
    config.parity    = PARITY_NONE;
    config.stop_bits = STOP_BITS_2;
	
	
    /* 查找系统中的串口设备 */
    serial = rt_device_find(SERVO_UART);
    if (!serial)
    {
        rt_kprintf("find %s failed!\n", SERVO_UART);
        return RT_ERROR;
    }
	
	rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config); 

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "servo_sem", 0, RT_IPC_FLAG_FIFO);
	
    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);
	
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);
	

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("servo", serial_thread_entry, RT_NULL, 1024, 25, 10);
	
	thread = rt_thread_create("servo_set", serial_send_thread_entry, RT_NULL, 1024, 25, 10);
	
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

INIT_DEVICE_EXPORT(uart_sample);


