#include "motion.h"


#include "transform.h"


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

        T_to_B(&target, &target);

        B_to_angle(&target);

        ptz_set(target);

        /* 延时等待转到位 */
        wait(1);

        //output(target);
    }
}



