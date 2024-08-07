#include "controltask/lpf.h"



double lpf_output(lpf_t *lpf, double input)
{
	lpf->output = lpf->output_l + LPF_alpha*(lpf->input - lpf->output_l);
	lpf->output_l = lpf->output;
	return lpf->output;
}



