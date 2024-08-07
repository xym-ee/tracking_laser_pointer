#include "motion.h"


#include "transform.h"


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

        T_to_B(&target, &target);

        B_to_angle(&target);

        ptz_set(target);

        /* ��ʱ�ȴ�ת��λ */
        wait(1);

        //output(target);
    }
}



