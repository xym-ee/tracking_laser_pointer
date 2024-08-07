#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>


#define TIME_STEP 16

#define PI 3.141593


/* motor init */
WbDeviceTag distance_sensor;

void distance_sensor_init(void)
{
    distance_sensor = wb_robot_get_device("distance_sensor");
    wb_distance_sensor_enable(distance_sensor, TIME_STEP);
}

/* motor init */
WbDeviceTag motor_yaw, motor_pit;

void motor_init(void)
{
    motor_yaw = wb_robot_get_device("motor_yaw");
    motor_pit = wb_robot_get_device("motor_pit");
}

inline double angle_to_deg(double angle)
{
    return angle * 180.0 / PI;
}

inline deg_to_angle(double deg)
{
    return deg * PI / 180.0;
}

/* 以角度值设置云台转角 */
void ptz_set_deg(double yaw, double pit)
{
    wb_motor_set_position(motor_yaw, deg_to_angle(yaw));
    wb_motor_set_position(motor_pit, deg_to_angle(pit));
}

/* 几何参数 */
#define h1      0.07f
#define d       1.0f
#define H1      0.25f
#define H2      0.25f

/* 逆变换的参数，这里写死了。用超声波读实际数据 */
double l = 0.6f;

typedef struct
{
    double x;
    double y;
    double z;
}
point_t;

/* 以弧度值设置云台转角，p.x 为偏转角，p.y 为俯仰角 */
void ptz_set(point_t p)
{
    wb_motor_set_position(motor_yaw, p.x);
    wb_motor_set_position(motor_pit, p.y);
}

void T_to_B(point_t* p)
{
    p->x = p->x - l;
    p->y = p->y + H1 + H2 - h1;
    p->z = -d;
}

void B_to_angle(point_t* p)
{
    double yaw, pit;

    double a = sqrt(p->x * p->x + p->z * p->z);

    yaw = acos(-p->z / a);

    pit = atan(p->y / a);

    /* 数学计算上的符号处理 */
    if (p->x < 0)
        p->x = -yaw;
    else
        p->x = yaw;

    p->y = pit;
}

/* 调试用，输出坐标值 */
void output(point_t p)
{
    printf("x : % f \ny : % f \nz : % f \n", p.x, p.y, p.z);
}

/* 效果和 delay() 一样  */
void wait(int times)
{
    while (times > 0)
    {
        times--;
        wb_robot_step(TIME_STEP);
    }
}

/* 从A点移动到B点，times为中间插入的目标点个数 */
void move(point_t A, point_t B, int times)
{
    point_t target;

    double x_length = B.x - A.x;
    double y_length = B.y - A.y;

    for (int i = 0; i <= times; i++)
    {
        target.x = ((double)i / times) * x_length + A.x;
        target.y = ((double)i / times) * y_length + A.y;

        T_to_B(&target);

        B_to_angle(&target);

        ptz_set(target);

        /* 延时等待转到位 */
        wait(1);

        //output(target);
    }
}

int main(int argc, char** argv)
{
    wb_robot_init();
    motor_init();
    distance_sensor_init();


    double distance = wb_distance_sensor_get_value(distance_sensor);

    printf("%f\n", distance);

    /*----------------------  移动到中点  ---------------------*/
    /*  坐标值为 {T} 参考系 */
    point_t point =
    {
        .x = 0,
        .y = 0,
        .z = 0,
    };

    T_to_B(&point);

    B_to_angle(&point);

    ptz_set(point);

    /* 等待执行到位 */
    wait(100);

    /*----------------------  移动到左上角点  ---------------------*/
    point = (point_t){
        .x = -0.25,
        .y = 0.25,
        .z = 0,
    };

    T_to_B(&point);

    B_to_angle(&point);

    ptz_set(point);

    wait(100);

    /*----------------------  画一个方框  ---------------------*/
    /* 坐标值为{T}坐标系，可用串口接收 */

    /* 画外框 */
    point_t recive[4] = {
        [0] = {-0.25,0.25,0.0},
        [1] = {0.25,0.25,0.0},
        [2] = {0.25,-0.25,0.0},
        [3] = {-0.25,-0.25,0.0},
    };

    /* 模拟一个A4值的4个点 */
    /*
    point_t recive[4] = {
        [0] = {-0.1,0.1,0.0},
        [1] = {0.2,0.3,0.0},
        [2] = {0.1,-0.15,0.0},
        [3] = {-0.15,-0.2,0.0},
    };
    */

    move(recive[0], recive[1], 300);
    move(recive[1], recive[2], 300);
    move(recive[2], recive[3], 300);
    move(recive[3], recive[0], 300);

    /* 暂停 */
    while (wb_robot_step(TIME_STEP) != -1)
    {
    }

    wb_robot_cleanup();
    return 0;
}
