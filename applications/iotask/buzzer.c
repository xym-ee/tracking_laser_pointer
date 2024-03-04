#include "iotask/buzzer.h"

rt_uint8_t buzzer_flag = 0;


static rt_thread_t buzzer_thread;



#define PWM_DEV_NAME        "pwm12"  /* PWM设备名称 */
#define PWM_DEV_CHANNEL     1       /* PWM通道 */

struct rt_device_pwm *pwm_dev;      /* PWM设备句柄 */

static void buzzer_thread_entry(void *parameter)
{
    while (1)
    {
        if(buzzer_flag == 1)
        {
            rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 500000, 25000);            
        }
        else
        {
            rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 500000, 0);
        }
        rt_thread_mdelay(100);
    }
}


/*-------------------------- 初始化 --------------------------*/

int buzzer_init(void)
{
    pwm_dev = (struct rt_device_pwm *)rt_device_find(PWM_DEV_NAME);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("pwm sample run failed! can't find %s device!\n", PWM_DEV_NAME);
        return RT_ERROR;
    }

    /* 设置PWM周期和脉冲宽度默认值 */
    rt_pwm_set(pwm_dev, PWM_DEV_CHANNEL, 500000, 0);
    /* 使能设备 */
    rt_pwm_enable(pwm_dev, PWM_DEV_CHANNEL);
        
    buzzer_thread = rt_thread_create("buzzer", buzzer_thread_entry, RT_NULL, 1024, 10, 10);
    
    if (buzzer_thread != RT_NULL)
    {
        rt_thread_startup(buzzer_thread);
    }
    else
    {
        rt_kprintf("create buzzer_thread failed!\n");
    }    
        
    return 0;
}

/* 导出到 msh 命令列表中 */
//MSH_CMD_EXPORT(can_init, can device sample);
INIT_APP_EXPORT(buzzer_init);



