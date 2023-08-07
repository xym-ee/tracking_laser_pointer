#ifndef __LPF_H__
#define __LPF_H__

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>




#define LPF_pi	3.141593
#define LPF_fc	1
#define LPF_Ts	0.005

#define LPF_alpha ((2.0*LPF_pi*LPF_fc*LPF_Ts)/(2.0*LPF_pi*LPF_fc*LPF_Ts+1.0))

typedef struct 
{
    double output;
    double output_l;
    double input;
} lpf_t;


double lpf_output(lpf_t *lpf, double input);


#endif



