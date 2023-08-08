#include "controltask/motion.h"

#include "iotask/sbus.h"
#include "iotask/motor_can.h"
#include "iotask/openmv.h"

#include "controltask/pid.h"


/*--------------------------  遥控器控制  ---------------------------*/

static void motion_thread_entry(void *parameter)
{
	rt_thread_mdelay(5000);
    
    /* 偏转控制器 */
    controller_t yaw_controller;
    controller_set_pid_parameter(&yaw_controller, 120, 0, 0);
    controller_set_output_limit(&yaw_controller, 5000);
    
    /* 俯仰控制器 */
    controller_t pit_controller;
    controller_set_pid_parameter(&pit_controller, 120, 0, 0);
    controller_set_output_limit(&pit_controller, 5000);
	
    while (1)
    {
        /* 自动控制 */
        motor.yaw.control.speed = controller_output(&yaw_controller, 90, pix.u);
        motor.pit.control.speed = -controller_output(&pit_controller, 90, pix.v);        
        
        
        
		/* 遥控器控制 */
//		motor.yaw.control.speed = (sbus.rh - SBUS_CH_OFFSET)*10000/SBUS_CH_LENGTH;
//		motor.pit.control.speed = (sbus.rv - SBUS_CH_OFFSET)*10000/SBUS_CH_LENGTH;

		rt_thread_mdelay(25);
    }
	
}

int motion_init(void)
{
    /* 创建 motion 线程 */
    rt_thread_t thread = rt_thread_create("motion", 
                                        motion_thread_entry, 
                                        RT_NULL, 
                                        MOTION_THREAD_STACK_SIZE, 
                                        MOTION_THREAD_PRIORITY, 
										MOTION_THREAD_TIMESLICE);
    
    /* 创建成功则启动线程 */
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }
	
	return 0;
}

/* 导出命令 or 自动初始化 */
INIT_APP_EXPORT(motion_init);



