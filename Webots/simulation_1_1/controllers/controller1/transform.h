#ifndef __TRANSFORM_H
#define __TRANSFORM_H





#define PI 3.141593





/* 几何参数 */
#define h1      0.07f
#define d       1.0f
#define H1      0.25f
#define H2      0.25f

typedef struct
{
    double x;
    double y;
    double z;
}
point_t;




void T_to_B(point_t* in,point_t* out);

void T_to_B(point_t* in, point_t* out);

void output(point_t p);

#endif // !__DEVICE_H
