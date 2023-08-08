#include "iotask/motor_can.h"

#include "stdlib.h"


motor_t motor = 
{
	.yaw.control.mode = MODE_SPEED,
	.pit.control.mode = MODE_SPEED,
};

/*-------------------------- CAN接收线程 *--------------------------*/

static rt_sem_t rx_sem = RT_NULL;     /* 用于接收消息的信号量 */

static rt_device_t can_dev = RT_NULL;            /* CAN 设备句柄 */

static rt_thread_t can_rx_thread;

/* 接收数据回调函数 */
static rt_err_t can_rx_call(rt_device_t dev, rt_size_t size)
{
    /* CAN 接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    rt_sem_release(rx_sem);

    return RT_EOK;
}

static void can_rx_thread_entry(void *parameter)
{
    struct rt_can_msg rxmsg = {0};

    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(can_dev, can_rx_call);

    while (1)
    {
        /* hdr 值为 - 1，表示直接从 uselist 链表读取数据 */
        rxmsg.hdr = -1;
        /* 阻塞等待接收信号量 */
        rt_sem_take(rx_sem, RT_WAITING_FOREVER);
        /* 从 CAN 读取一帧数据 */
        rt_device_read(can_dev, 0, &rxmsg, sizeof(rxmsg));
        
        /* 俯仰电机 */
        if (rxmsg.id == 0x142)
        {
			//motor.pit.sensor.speed = (rt_int16_t)((rxmsg.data[5]<<8) | rxmsg.data[4]);

        }
        
        /* 偏转电机 */
        if (rxmsg.id == 0x141)
        {
			//motor.yaw.sensor.speed = (rt_int16_t)((rxmsg.data[5]<<8) | rxmsg.data[4]);
        }
    }
}


/*---------------------- CAN 发送线程 ---------------------*/

static rt_thread_t can_tx_thread = RT_NULL;

static void can_tx_thread_entry(void *parameter)
{
    struct rt_can_msg msg = {0};
    rt_size_t   size;               /* 接收发送状态 */
    
	rt_int32_t speedControl;

    /* 速度帧设置 */
    msg.ide = RT_CAN_STDID;         /* 标准格式 */
    msg.rtr = RT_CAN_DTR;           /* 数据帧 */
    msg.len = 8;                    /* 数据长度为 8 */
	
	/* 转矩控制的固定数据位 msg.data[4][5] 转矩位 */
	msg.data[0] = 0xA2;             /* 操作码 */
	 
	msg.data[1] = 0x00;
	msg.data[2] = 0x00; 
	msg.data[3] = 0x00;

    rt_thread_mdelay(4000);
    
		
    while(1)
    {
		speedControl = motor.pit.control.speed;
//		rt_kprintf("%d\n", iqControl);
		
		msg.id   = 0x142;	
		msg.data[4] = *(rt_uint8_t *)(&speedControl);
		msg.data[5] = *((rt_uint8_t *)(&speedControl)+1);
        msg.data[6] = *((rt_uint8_t *)(&speedControl)+2);
        msg.data[7] = *((rt_uint8_t *)(&speedControl)+3);
		
		size = rt_device_write(can_dev, 0, &msg, sizeof(msg));
//		if (size == 0)
//		{
//			rt_kprintf("can dev write data failed in left!\n");
//		}
		
		rt_thread_mdelay(5);
		
		
		speedControl = motor.yaw.control.speed;
//		rt_kprintf("%d\n", iqControl);
		
		msg.id      = 0x141;
		msg.data[4] = *(rt_uint8_t *)(&speedControl);
		msg.data[5] = *((rt_uint8_t *)(&speedControl)+1);
        msg.data[6] = *((rt_uint8_t *)(&speedControl)+2);
        msg.data[7] = *((rt_uint8_t *)(&speedControl)+3);
		
		size = rt_device_write(can_dev, 0, &msg, sizeof(msg));
//		if (size == 0)
//		{
//			rt_kprintf("can dev write data failed in right!\n");
//		}
		
        rt_thread_mdelay(5);
    }
}

/*-------------------------- 初始化 --------------------------*/

int can_init(void)
{
    rt_err_t res;

    /* 查找 CAN 设备 */
    can_dev = rt_device_find(CAN_DEV_NAME);
    if (!can_dev)
    {
        rt_kprintf("find %s failed!\n", CAN_DEV_NAME);
        return RT_ERROR;
    }
    /* 以中断接收及发送方式打开 CAN 设备 */
    res = rt_device_open(can_dev, RT_DEVICE_FLAG_INT_TX | RT_DEVICE_FLAG_INT_RX);
    res = rt_device_control(can_dev, RT_CAN_CMD_SET_BAUD, (void *)CAN1MBaud);
    RT_ASSERT(res == RT_EOK);
    
    rx_sem = rt_sem_create("rx_sem", 0, RT_IPC_FLAG_FIFO);

    /* 创建数据接收线程 */
    can_rx_thread = rt_thread_create("can_rx", can_rx_thread_entry, RT_NULL, CAN_RX_STACK_SIZE, CAN_RX_PRIORITY, CAN_RX_TIMESLICE);
    if (can_rx_thread != RT_NULL)
    {
        rt_thread_startup(can_rx_thread);
    }
    else
    {
        rt_kprintf("create can_rx thread failed!\n");
    }

    /* 创建数据发送线程 */
    can_tx_thread = rt_thread_create("can_tx", can_tx_thread_entry, RT_NULL, CAN_TX_STACK_SIZE, CAN_TX_PRIORITY, CAN_TX_TIMESLICE);
    if (can_rx_thread != RT_NULL)
    {
        rt_thread_startup(can_tx_thread);
    }
    else
    {
        rt_kprintf("create can_tx thread failed!\n");
    }
    
    return 0;
}

/* 导出到 msh 命令列表中 */
//MSH_CMD_EXPORT(can_init, can device sample);
INIT_APP_EXPORT(can_init);




