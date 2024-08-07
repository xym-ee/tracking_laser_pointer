#include "device.h"

/* motor init */
WbDeviceTag motor_yaw, motor_pit;

void motor_init(void)
{
    motor_yaw = wb_robot_get_device("motor_yaw");
    motor_pit = wb_robot_get_device("motor_pit");
}


/* �Ի���ֵ������̨ת�ǣ�p.x Ϊƫת�ǣ�p.y Ϊ������ */
void ptz_set(ptz_t ptz)
{
    wb_motor_set_position(motor_yaw, ptz.yaw);
    wb_motor_set_position(motor_pit, ptz.pit);
}


void device_init(void)
{
    wb_robot_init();
    motor_init();
}

