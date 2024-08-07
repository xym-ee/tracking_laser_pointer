#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/camera.h>

#define TIME_STEP 16

#define PI 3.141593


/* device init */
WbDeviceTag motor_yaw, motor_pit, camera;

void device_init(void)
{
    motor_yaw = wb_robot_get_device("motor_yaw");
    motor_pit = wb_robot_get_device("motor_pit");

    camera = wb_robot_get_device("camera");
    wb_camera_enable(camera, TIME_STEP);
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

void ptz_set_velocity(double yaw,double pit)
{
    wb_motor_set_velocity(motor_yaw, yaw);
    wb_motor_set_velocity(motor_pit, pit);
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


#define IMAGE_WIDTH 160
#define IMAGE_HEIGH 120

typedef struct{ 
    int u;
    int v;
} pix_t;

pix_t pix = { IMAGE_WIDTH / 2,IMAGE_HEIGH / 2 };

void find_red_point(void)
{
    const unsigned char* image = wb_camera_get_image(camera);

    for (int x = 0; x < IMAGE_WIDTH; x++)
    {
        for (int y = 0; y < IMAGE_HEIGH; y++)
        {
            int r = wb_camera_image_get_red(image, IMAGE_WIDTH, x, y);

            if (r > 200)
            {
                pix.u = x;
                pix.v = y;

                return;
            }
        }
    }
}


int main(int argc, char** argv)
{
    wb_robot_init();
    device_init();

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

    /* 电机调整为转速控制模式 */
    wb_motor_set_position(motor_yaw, INFINITY);
    wb_motor_set_position(motor_pit, INFINITY);

    /*----------------------  跟踪红点  ---------------------*/


    while (wb_robot_step(TIME_STEP) != -1)
    {
        find_red_point();

        double yaw_v = -0.05 * (IMAGE_WIDTH / 2 - (pix.u + 1));
        double pit_v = 0.05 * (IMAGE_HEIGH / 2 - (pix.v - 4));

        ptz_set_velocity(yaw_v,pit_v);

    }


    /* 暂停 */
    while (wb_robot_step(TIME_STEP) != -1)
    {
    }

    wb_robot_cleanup();
    return 0;
}
