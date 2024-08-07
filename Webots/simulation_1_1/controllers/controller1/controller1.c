

#include "device.h"
#include "transform.h"
#include "motion.h"

#define TIME_STEP 16


/* 效果和 delay() 一样  */
void wait(int times)
{
    while (times > 0)
    {
        times--;
        wb_robot_step(TIME_STEP);
    }
}



int main(int argc, char** argv)
{
    device_init();

    ptz_t ptz_output;

    /*----------------------  移动到中点  ---------------------*/
    /*  坐标值为 {T} 参考系 */ 
    point_t point =
    {
        .x = 0,
        .y = 0,
        .z = 0,
    };

    T_to_B(&point, &point);

    B_to_angle(&point, &ptz_output);

    ptz_set(ptz_output);

    /* 等待执行到位 */
    wait(100);

    /*----------------------  移动到左上角点  ---------------------*/
    point = (point_t){
        .x = -0.25,
        .y = 0.25,
        .z = 0,
    };

    T_to_B(&point, &point);

    B_to_angle(&point, &ptz_output);

    ptz_set(ptz_output);

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
