#include "Filter.h"
#include "Util.h"
#include "Vector.h"
#include "GrayBMP.h"
#include "ImageProcess.h"

void Filter::Gaussian(const GrayBMP &src, GrayBMP &dst, const GaussianModel& gaussian)
{
	float w = 1 / gaussian.gaussian.Sum();
	for (int i = 0; i < src.getWidth(); ++i)
	{
		for (int j = 0; j < src.getHeight(); ++j)
		{
			if (Limit::OutOfRange(i, gaussian.range, 0, src.getWidth() - 1)
					|| Limit::OutOfRange(j, gaussian.range, 0,
							src.getHeight() - 1))
			{
				dst(i, j) = src(i, j);
			}
			else
			{
				Vector<float> origin;
				origin = ImgProcess::GetSquare(src,i, j, gaussian.range);
				dst(i, j) = (Byte) (origin * gaussian.gaussian * w);
			}
		}
	}
}

void Filter::Gaussian_Applyto(GrayBMP &target, const GaussianModel& gaussian)
{
	GrayBMP src(target);

	float w = 1 / gaussian.gaussian.Sum();
	for (int i = 0; i < src.getWidth(); ++i)
	{
		for (int j = 0; j < src.getHeight(); ++j)
		{
			if (Limit::OutOfRange(i, gaussian.range, 0, src.getWidth() - 1)
					|| Limit::OutOfRange(j, gaussian.range, 0,
							src.getHeight() - 1))
			{
				target(i, j) = src(i, j);
			}
			else
			{
				Vector<float> origin;
				origin = ImgProcess::GetSquare(src,i, j, gaussian.range);
				target(i, j) = (Byte) (origin * gaussian.gaussian * w);
			}
		}
	}
}

void Filter::Median(const GrayBMP &src, GrayBMP &dst, int Range)
{
	for (int i = 0; i < src.getWidth(); ++i)
	{
		for (int j = 0; j < src.getHeight(); ++j)
		{
			if (Limit::OutOfRange(i, Range, 0, src.getWidth() - 1)
					|| Limit::OutOfRange(j, Range, 0, src.getHeight() - 1))
			{
				dst(i, j) = src(i, j);
			}
			else
			{
				Vector<float> origin;
				origin = ImgProcess::GetSquare(src,i, j, Range);
				origin.Sort();
				dst(i, j) = (Byte) (origin[Range * Range / 2 + 1]);
			}
		}
	}
}
