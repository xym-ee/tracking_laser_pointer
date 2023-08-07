#ifndef __IMU_H__
#define __IMU_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#define IMU_UART       "uart2"


#define IMU_THREAD_PRIORITY         2
#define IMU_THREAD_STACK_SIZE       1024
#define IMU_THREAD_TIMESLICE        10



typedef struct 
{
	rt_int32_t roll;	
	rt_int32_t pitch;
	rt_int32_t yaw;
} 
imu_t;

extern imu_t imu;

#endif



