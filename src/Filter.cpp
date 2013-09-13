#include "Filter.h"
#include "Util.h"
#include "Vector.h"
#include "GrayBMP.h"
#include "ImageProcess.h"

void Filter::Gaussian(const GrayBMP &src, GrayBMP &dst,
        const GaussianModel& gaussian)
{
    float w = 1 / gaussian.gaussian.sum();

    for (int j = 0; j < src.getHeight(); ++j)
    {
        for (int i = 0; i < src.getWidth(); ++i)

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
                origin = ImgProcess::GetSquare(src, i, j, gaussian.range);
                dst(i, j) = (Byte) (origin * gaussian.gaussian * w);
            }
        }
    }
}

void Filter::ConvApplyto(GrayBMP &bmp, int Filter[], int length)
{
    GrayBMP src(bmp);
    Conv(src,bmp,Filter,length);
}
void Filter::Conv(const GrayBMP &src, GrayBMP &dst, int Filter[], int length)
{
    int width = src.getWidth(), height = src.getHeight();
    int R = length / 2;
    GrayBMP Cache(width, height);
    int sum = 0;
    for (int i = 0; i < length; ++i)
        sum += Filter[i];
//第一次X向处理
    for (int y = 0; y < height; y++)
    {
        for (int x = R; x < width - R; x++)
        {
            int F = (src(x, y) * Filter[R]);
            for (int i = 1; i <= R; i++)
            {
                F = F + src(x - i, y) * Filter[R - i]
                        + src(x + i, y) * Filter[R - i];
            }
            Cache(x, y) = F / sum;
        }
    }
//第二次Y向处理
    for (int y = R; y < height - R; y++)
    {
        for (int x = R; x < width - R; x++)
        {
            int F = Cache(x, y) * Filter[R];
            for (int i = 1; i <= R; i++)
            {
                F = F + Cache(x, y - i) * Filter[R - i]
                        + Cache(x, y + i) * Filter[R - i];
            }
            dst(x, y) = (int) F / sum;
        }
    }
}

void Filter::GaussianApplyto(GrayBMP &target, const GaussianModel& gaussian)
{
    GrayBMP src(target);

    float w = 1 / gaussian.gaussian.sum();
    for (int j = 0; j < src.getHeight(); ++j)
    {
        for (int i = 0; i < src.getWidth(); ++i)
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
                origin = ImgProcess::GetSquare(src, i, j, gaussian.range);
                target(i, j) = (Byte) (origin * gaussian.gaussian * w);
            }
        }
    }
}

void Filter::Median(const GrayBMP &src, GrayBMP &dst, int Range)
{
    for (int j = 0; j < src.getHeight(); ++j)
    {
        for (int i = 0; i < src.getWidth(); ++i)
        {
            if (Limit::OutOfRange(i, Range, 0, src.getWidth() - 1)
                    || Limit::OutOfRange(j, Range, 0, src.getHeight() - 1))
            {
                dst(i, j) = src(i, j);
            }
            else
            {
                Vector<float> origin;
                origin = ImgProcess::GetSquare(src, i, j, Range);
                origin.Sort();
                dst(i, j) = (Byte) (origin[Range * Range / 2 + 1]);
            }
        }
    }
}
