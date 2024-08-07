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
	double theta_x;	
	double theta_y;
	double theta_z;	
	
	double omega_x;	
	double omega_y;
	double omega_z;	
		
} 
imu_t;

extern imu_t imu;

#endif



