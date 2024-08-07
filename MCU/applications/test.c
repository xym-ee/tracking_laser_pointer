#include "test.h"




static rt_thread_t thread1,thread2;

static void thread_entry1(void *parameter)
{
    while (1)
    {
        rt_kprintf("A");
//        rt_thread_mdelay(1);

    }
}

static void thread_entry2(void *parameter)
{
    while (1)
    {
        rt_kprintf("B");
//        rt_thread_mdelay(1);
    }
}

/*-------------------------- 初始化 --------------------------*/

int init(void)
{

    thread1 = rt_thread_create("th1", thread_entry1, RT_NULL, 1024, 5, 2);
    if (thread1 != RT_NULL)
    {
        rt_thread_startup(thread1);
    }
    else
    {
        rt_kprintf("create can_rx thread failed!\n");
    }

    /* 创建数据发送线程 */
    thread2 = rt_thread_create("th2", thread_entry2, RT_NULL, 1024, 5, 2);
    if (thread2 != RT_NULL)
    {
        rt_thread_startup(thread2);
    }
    else
    {
        rt_kprintf("create can_tx thread failed!\n");
    }
    
    return 0;
}

/* 导出到 msh 命令列表中 */
//MSH_CMD_EXPORT(can_init, can device sample);
INIT_APP_EXPORT(init);








