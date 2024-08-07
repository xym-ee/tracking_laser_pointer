#ifndef __DEVICE_H
#define __DEVICE_H


#include <webots/robot.h>
#include <webots/motor.h>



typedef struct
{
    double yaw;
    double pit;
} ptz_t;

void device_init(void);


void ptz_set(ptz_t ptz);

#endif // !__DEVICE_H
