#ifndef _INC_FILTER_H_
#define _INC_FILTER_H_


class GrayBMP;

void Filter_Gaussian(GrayBMP &src,GrayBMP &dst, int Range, float Var);
void Filter_Median(GrayBMP &src,GrayBMP &dst, int Range);

#endif