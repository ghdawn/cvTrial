#include "Filter.h"
#include "Util.h"
#include "Vector.h"
#include "GrayBMP.h"


void Filter_Gaussian(const GrayBMP &src, GrayBMP &dst, const GaussianModel& gaussian)
{
	float w = 1 / gaussian.gaussian.sum();
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if (Limit::OutOfRange(i, gaussian.range, 0, src.TellWidth() - 1)
					|| Limit::OutOfRange(j, gaussian.range, 0,
							src.TellHeight() - 1))
			{
				dst(i, j) = src(i, j);
			}
			else
			{
				Vector<float> origin;
				origin = src.GetSquare(i, j, gaussian.range);
				dst(i, j) = (Byte) (origin * gaussian.gaussian * w);
			}
		}
	}
}

void Filter_Gaussian_Applyto(GrayBMP &target, const GaussianModel& gaussian)
{
	GrayBMP src(target);

	float w = 1 / gaussian.gaussian.sum();
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if (Limit::OutOfRange(i, gaussian.range, 0, src.TellWidth() - 1)
					|| Limit::OutOfRange(j, gaussian.range, 0,
							src.TellHeight() - 1))
			{
				target(i, j) = src(i, j);
			}
			else
			{
				Vector<float> origin;
				origin = src.GetSquare(i, j, gaussian.range);
				target(i, j) = (Byte) (origin * gaussian.gaussian * w);
			}
		}
	}
}

void Filter_Median(const GrayBMP &src, GrayBMP &dst, int Range)
{
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if (Limit::OutOfRange(i, Range, 0, src.TellWidth() - 1)
					|| Limit::OutOfRange(j, Range, 0, src.TellHeight() - 1))
			{
				dst(i, j) = src(i, j);
			}
			else
			{
				Vector<float> origin;
				origin = src.GetSquare(i, j, Range);
				origin.Sort();
				dst(i, j) = (Byte) (origin[Range * Range / 2 + 1]);
			}
		}
	}
}
