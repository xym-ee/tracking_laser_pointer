#ifndef __SBUS_H__
#define __SBUS_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#define SBUS_UART       "uart1"


#define SBUS_THREAD_PRIORITY         3
#define SBUS_THREAD_STACK_SIZE       1024
#define SBUS_THREAD_TIMESLICE        10


#define SBUS_CH_MIN             ((rt_uint16_t)353 )
#define SBUS_CH_OFFSET          ((rt_uint16_t)1024)
#define SBUS_CH_MAX             ((rt_uint16_t)1695)
#define SBUS_CH_LENGTH          ((rt_uint16_t)(SBUS_CH_MAX - SBUS_CH_OFFSET))

#define SBUS_SW_UP              ((rt_uint16_t)353)
#define SBUS_SW_MID             ((rt_uint16_t)1024)
#define SBUS_SW_DOWN            ((rt_uint16_t)1695)


/* ----------------------- Data Struct ------------------------------------- */

/* 通道与遥控器的对应关系 */
typedef union
{
    struct
    {
        rt_uint16_t     ch1;
        rt_uint16_t     ch2;
        rt_uint16_t     ch3;
        rt_uint16_t     ch4;
        rt_uint16_t     ch5;
        rt_uint16_t     ch6;
        rt_uint16_t     ch7;
        rt_uint16_t     ch8;
        rt_uint16_t     ch9;
        rt_uint16_t     ch10;
        rt_uint16_t     ch11;
        rt_uint16_t     ch12;
        rt_uint16_t     ch13;
        rt_uint16_t     ch14;
        rt_uint16_t     ch15;
        rt_uint16_t     ch16;
    };
    struct
    {
        rt_uint16_t     rh;
        rt_uint16_t     rv;
        rt_uint16_t     lv;
        rt_uint16_t     lh;
        rt_uint16_t     ld;
        rt_uint16_t     rd;
        rt_uint16_t     sa;
        rt_uint16_t     sb;
        rt_uint16_t     sc;
        rt_uint16_t     sd;
        rt_uint16_t     se;
        rt_uint16_t     sf;
        rt_uint16_t     sg;
        rt_uint16_t     sh;
        rt_uint16_t     ls;
        rt_uint16_t     rs;
    };    
}sbus_t;


extern sbus_t sbus;


#endif

