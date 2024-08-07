#include "transform.h"
#include "device.h"

#include <stdio.h>
#include <math.h>


inline double angle_to_deg(double angle)
{
    return angle * 180.0 / PI;
}

inline double deg_to_angle(double deg)
{
    return deg * PI / 180.0;
}



void T_to_B(point_t* in, point_t* out)
{
    out->y = in->y + H1 + H2 - h1;
    out->z = -d;


    //p->y = p->y + H1 + H2 - h1;
    //p->z = -d;
}

void B_to_angle(point_t* in, ptz_t *out)
{
    double yaw, pit;

    double a = sqrt(in->x * in->x + in->z * in->z);

    yaw = acos(-in->z / a);

    pit = atan(in->y / a);

    /* 数学计算上的符号处理 */
    if (in->x < 0)
        out->yaw = -yaw;
    else
        out->yaw = yaw;

    out->pit = pit;
}

/* 调试用，输出坐标值 */
void output(point_t p)
{
    printf("x : % f \ny : % f \nz : % f \n", p.x, p.y, p.z);
}

