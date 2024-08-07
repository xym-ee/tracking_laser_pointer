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

/* �ԽǶ�ֵ������̨ת�� */
void ptz_set_deg(double yaw, double pit)
{
    wb_motor_set_position(motor_yaw, deg_to_angle(yaw));
    wb_motor_set_position(motor_pit, deg_to_angle(pit));
}

/* ���β��� */
#define h1      0.07f
#define d       1.0f
#define H1      0.25f
#define H2      0.25f

/* ��任�Ĳ���������д���ˡ��ó�������ʵ������ */
double l = 0.6f;

typedef struct
{
    double x;
    double y;
    double z;
}
point_t;

/* �Ի���ֵ������̨ת�ǣ�p.x Ϊƫת�ǣ�p.y Ϊ������ */
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

    /* ��ѧ�����ϵķ��Ŵ��� */
    if (p->x < 0)
        p->x = -yaw;
    else
        p->x = yaw;

    p->y = pit;
}

/* �����ã��������ֵ */
void output(point_t p)
{
    printf("x : % f \ny : % f \nz : % f \n", p.x, p.y, p.z);
}

/* Ч���� delay() һ��  */
void wait(int times)
{
    while (times > 0)
    {
        times--;
        wb_robot_step(TIME_STEP);
    }
}

/* ��A���ƶ���B�㣬timesΪ�м�����Ŀ������ */
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

        /* ��ʱ�ȴ�ת��λ */
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

    /*----------------------  �ƶ����е�  ---------------------*/
    /*  ����ֵΪ {T} �ο�ϵ */
    point_t point =
    {
        .x = 0,
        .y = 0,
        .z = 0,
    };

    T_to_B(&point);

    B_to_angle(&point);

    ptz_set(point);

    /* �ȴ�ִ�е�λ */
    wait(100);

    /* �������Ϊת�ٿ���ģʽ */
    wb_motor_set_position(motor_yaw, INFINITY);
    wb_motor_set_position(motor_pit, INFINITY);

    /*----------------------  ���ٺ��  ---------------------*/


    while (wb_robot_step(TIME_STEP) != -1)
    {
        find_red_point();

        double yaw_v = -0.05 * (IMAGE_WIDTH / 2 - (pix.u + 1));
        double pit_v = 0.05 * (IMAGE_HEIGH / 2 - (pix.v - 4));

        ptz_set_velocity(yaw_v,pit_v);

    }


    /* ��ͣ */
    while (wb_robot_step(TIME_STEP) != -1)
    {
    }

    wb_robot_cleanup();
    return 0;
}
