#include "GrayBMP.h"

void Filter_Gaussian(GrayBMP &src,GrayBMP &dst, int Range, float Var);
void Filter_Median(GrayBMP &src,GrayBMP &dst, int Range);