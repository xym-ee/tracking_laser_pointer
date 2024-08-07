#ifndef __LED_H__
#define __LED_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#define LED_FLOW_PRIORITY         29
#define LED_FLOW_STACK_SIZE       256
#define LED_FLOW_TIMESLICE        5

#define LED_FAST_PRIORITY         29
#define LED_FAST_STACK_SIZE       256
#define LED_FAST_TIMESLICE        5

#define LED_SLOW_PRIORITY         29
#define LED_SLOW_STACK_SIZE       256
#define LED_SLOW_TIMESLICE        5


#define LED1_PIN    GET_PIN(G, 1)
#define LED2_PIN    GET_PIN(G, 2)
#define LED3_PIN    GET_PIN(G, 3)
#define LED4_PIN    GET_PIN(G, 4)
#define LED5_PIN    GET_PIN(G, 5)
#define LED6_PIN    GET_PIN(G, 6)
#define LED7_PIN    GET_PIN(G, 7)
#define LED8_PIN    GET_PIN(G, 8)
#define LED9_PIN    GET_PIN(E, 11)
#define LED10_PIN   GET_PIN(F, 14)


#endif



