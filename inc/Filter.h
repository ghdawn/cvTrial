#ifndef _INC_FILTER_H_
#define _INC_FILTER_H_
#include "Vector.h"

class GrayBMP;

class GaussianModel
{
public:

	GaussianModel(int Range, float Var)
	{
		int k = 0;
		range = Range;
		var = Var;
		int mid = Range / 2;
		gaussian.allocate(Range * Range);
		for (int i = 0; i < Range; ++i)
		{
			for (int j = 0; j < Range; ++j)
			{
				gaussian[k++] = Math::Gaussian(i - mid, j - mid, Var);
			}
		}
	}
	~GaussianModel()
	{
		gaussian.Dispose();
	}

	/* data */
	int range;
	float var;
	Vector<float> gaussian;
};
void Filter_Gaussian_Applyto(GrayBMP &target, const GaussianModel& gaussian);
void Filter_Gaussian(const GrayBMP &src, GrayBMP &dst, const GaussianModel& gaussian);
void Filter_Median(const GrayBMP &src, GrayBMP &dst, int Range);

#endif
