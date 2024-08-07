#include "led.h"


/*-------------------------------   线程1：流水灯   -------------------------------*/
static rt_thread_t led_flow = RT_NULL;

static void led_flow_thread(void *parameter)
{
    rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED4_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED5_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED6_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED7_PIN, PIN_MODE_OUTPUT);
    rt_pin_mode(LED8_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LED1_PIN, PIN_HIGH);
    rt_pin_write(LED2_PIN, PIN_HIGH);
    rt_pin_write(LED3_PIN, PIN_HIGH);
    rt_pin_write(LED4_PIN, PIN_HIGH);
    rt_pin_write(LED5_PIN, PIN_HIGH);
    rt_pin_write(LED6_PIN, PIN_HIGH);
    rt_pin_write(LED7_PIN, PIN_HIGH);
    rt_pin_write(LED8_PIN, PIN_HIGH);

    
    rt_thread_mdelay(5000);
    
    
    while (1)
    { /* 依次点亮LED */
        rt_pin_write(LED1_PIN, PIN_LOW);
        rt_thread_mdelay(100);
        rt_pin_write(LED2_PIN, PIN_LOW);
        rt_thread_mdelay(100);
        rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(100);
        rt_pin_write(LED4_PIN, PIN_LOW);
        rt_thread_mdelay(100);
        rt_pin_write(LED5_PIN, PIN_LOW);
        rt_thread_mdelay(100);
        rt_pin_write(LED6_PIN, PIN_LOW);
        rt_thread_mdelay(100);
        rt_pin_write(LED7_PIN, PIN_LOW);
        rt_thread_mdelay(100);
        rt_pin_write(LED8_PIN, PIN_LOW);

        rt_thread_mdelay(1000); /* 1s后全部熄灭 */
        rt_pin_write(LED1_PIN, PIN_HIGH);
        rt_pin_write(LED2_PIN, PIN_HIGH);
        rt_pin_write(LED3_PIN, PIN_HIGH);
        rt_pin_write(LED4_PIN, PIN_HIGH);
        rt_pin_write(LED5_PIN, PIN_HIGH);
        rt_pin_write(LED6_PIN, PIN_HIGH);
        rt_pin_write(LED7_PIN, PIN_HIGH);
        rt_pin_write(LED8_PIN, PIN_HIGH);

        rt_thread_mdelay(1000);
    }
}

int led_flow_start(void)
{
  led_flow = rt_thread_create("led_flow",
                              led_flow_thread, 
                              RT_NULL,
                              LED_FLOW_STACK_SIZE,
                              LED_FLOW_PRIORITY, 
                              LED_FLOW_TIMESLICE); 

  if (led_flow != RT_NULL)
      rt_thread_startup(led_flow);
  
  return 0;
}

//MSH_CMD_EXPORT(led_flow_start, led flow start);
INIT_APP_EXPORT(led_flow_start);


/*-------------------------------   线程2：快闪   -------------------------------*/
static rt_thread_t led_fast_flash = RT_NULL;

static void led_fast_flash_thread(void *parameter)
{
    rt_pin_mode(LED9_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LED9_PIN, PIN_HIGH);

    
    rt_thread_mdelay(5000);
    
    while (1)
    {
      rt_pin_write(LED9_PIN, PIN_LOW);
      rt_thread_mdelay(100);

      rt_pin_write(LED9_PIN, PIN_HIGH);
      rt_thread_mdelay(200);
    }
}

int led_fast_start(void)
{
    led_fast_flash = rt_thread_create("led_fast",
                                      led_fast_flash_thread, 
                                      RT_NULL,
                                      LED_FAST_STACK_SIZE,
                                      LED_FAST_PRIORITY, 
                                      LED_FAST_TIMESLICE);

    if (led_fast_flash != RT_NULL)
        rt_thread_startup(led_fast_flash);
    
    return 0;
}

//MSH_CMD_EXPORT(led_fast_start, led fast flash start);
INIT_APP_EXPORT(led_fast_start);

/*-------------------------------   线程3：慢闪   -------------------------------*/
static rt_thread_t led_slow_flash = RT_NULL;

static void led_slow_flash_thread(void *parameter)
{
    rt_pin_mode(LED10_PIN, PIN_MODE_OUTPUT);
    rt_pin_write(LED10_PIN, PIN_HIGH);

    
    
    rt_thread_mdelay(5000);
    
    while (1)
    {
      rt_pin_write(LED10_PIN, PIN_LOW);
      rt_thread_mdelay(200);

      rt_pin_write(LED10_PIN, PIN_HIGH);
      rt_thread_mdelay(800);
    }
}

int led_slow_start(void)
{
    led_slow_flash = rt_thread_create("led_slow",
                                      led_slow_flash_thread, 
                                      RT_NULL,
                                      LED_SLOW_STACK_SIZE,
                                      LED_SLOW_PRIORITY, 
                                      LED_SLOW_TIMESLICE);

    if (led_slow_flash != RT_NULL)
        rt_thread_startup(led_slow_flash);
    return 0;
}

//MSH_CMD_EXPORT(led_slow_start, led slow flash start);
INIT_APP_EXPORT(led_slow_start);



