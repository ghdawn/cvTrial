#include "ImageProcess.h"
#include "Util.h"
#include "GrayBMP.h"
#include "Vector.h"
#include "Structure/Rect.h"

void ImgProcess::Dx(const GrayBMP &src, GrayBMP &dx)
{
    if (!dx.MatchSize(src.getWidth(), src.getHeight()))
    {
        dx.SetSize(src.getWidth(), src.getHeight());
    }
    for (int j = 0; j < src.getHeight(); ++j)
    {
        for (int i = 0; i < src.getWidth(); ++i)
        {
            if ((i == 0) || (j == 0) || (i == (src.getWidth() - 1))
                    || (j == (src.getHeight() - 1)))
            {
                dx(i, j) = src(i, j);
            }
            else
            {
                int TempBYTE = (src(i + 1, j - 1) - src(i - 1, j - 1))
                        + 2 * (src(i + 1, j) - src(i - 1, j))
                        + (src(i + 1, j + 1) - src(i - 1, j + 1));
                // int TempBYTE = Limit::Round((src(i + 1, j) - src(i - 1, j)) / 2.0);
                // int TempBYTE=(src(i+1,j-1)+2*src(i+1,j)+src(i+1,j+1)+(-1)*src(i-1,j-1)+(-2)*src(i-1,j)+(-1)*src(i-1,j+1));
                dx(i, j) = Limit::GrayByte(TempBYTE >> 3);
            }
        }
    }
}

void ImgProcess::Dy(const GrayBMP &src, GrayBMP &dy)
{
    if (!dy.MatchSize(src.getWidth(), src.getHeight()))
    {
        dy.SetSize(src.getWidth(), src.getHeight());
    }
    for (int j = 0; j < src.getHeight(); ++j)
    {
        for (int i = 0; i < src.getWidth(); ++i)
        {
            if ((i == 0) || (j == 0) || (i == (src.getWidth() - 1))
                    || (j == (src.getHeight() - 1)))
            {
                dy(i, j) = src(i, j);
            }
            else
            {
                int TempBYTE = (src(i - 1, j + 1) - src(i - 1, j - 1))
                        + 2 * (src(i, j + 1) - src(i, j - 1))
                        + (src(i + 1, j + 1) - src(i + 1, j - 1));
                //int TempBYTE = Limit::Round((src(i, j + 1) - src(i, j - 1)) / 2.0);
                dy(i, j) = Limit::GrayByte(TempBYTE >> 3);
            }
        }
    }
}

void ImgProcess::Sobel(const GrayBMP &src, GrayBMP &result)
{
    if (!result.MatchSize(src.getWidth(), src.getHeight()))
    {
        result.SetSize(src.getWidth(), src.getHeight());
    }
    for (int j = 0; j < src.getHeight(); ++j)
    {
        for (int i = 0; i < src.getWidth(); ++i)
        {
            if ((i == 0) || (j == 0) || (i == (src.getWidth() - 1))
                    || (j == (src.getHeight() - 1)))
            {
                result(i, j) = src(i, j);
            }
            else
            {
                int dx = (src(i + 1, j - 1) - src(i - 1, j - 1))
                        + 2 * (src(i + 1, j) - src(i - 1, j))
                        + (src(i + 1, j + 1) - src(i - 1, j + 1));
                int dy = (src(i - 1, j + 1) - src(i - 1, j - 1))
                        + 2 * (src(i, j + 1) - src(i, j - 1))
                        + (src(i + 1, j + 1) - src(i + 1, j - 1));
                result(i, j) = Limit::GrayByte(Math::Abs(dx) + Math::Abs(dy));
            }
        }
    }
}

void ImgProcess::DownSampling(const GrayBMP & src, GrayBMP& dst, int scale)
{
    if (!dst.MatchSize(src.getWidth() / scale, src.getHeight() / scale))
    {
        dst.SetSize(src.getWidth() / scale, src.getHeight() / scale);
    }
    for (int j = 0; j < dst.getHeight(); ++j)
    {
        for (int i = 0; i < dst.getWidth(); ++i)
        {
            dst(i, j) = src(i * scale, j * scale);
        }
    }
}

void ImgProcess::Scaling(const GrayBMP& src, GrayBMP& dst)
{
    int width = dst.getWidth(), height = dst.getHeight();
    float fw = float(src.getWidth()) / width;
    float fh = float(src.getHeight()) / height;
    float x, y;
    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            x = i * fw;
            y = j * fh;
            dst(i, j) = Interpolation(src, x, y);
        }
    }
}

Vector<float> ImgProcess::GetSquare(const GrayBMP& src, int x, int y, int Range)
{
    Vector<float> vec(Range * Range);
    int k = 0;
    Range /= 2;
    for (int j = y - Range; j <= y + Range; ++j)
    {
        for (int i = x - Range; i <= x + Range; ++i)
        {

            vec[k++] = src(i, j);
        }
    }
    return vec;
}

Vector<float> ImgProcess::toVector(const GrayBMP& src)
{
    Vector<float> vec(src.getWidth() * src.getHeight());
    int k = 0;
    for (int j = 0; j < src.getHeight(); ++j)
    {
        for (int i = 0; i < src.getWidth(); ++i)
        {
            vec[k++] = src(i, j);
        }
    }
    return vec;
}

void ImgProcess::GetSquare(const GrayBMP& src, GrayBMP& dst, Rect rect)
{
    if (!dst.MatchSize(rect.getWidth(), rect.getHeight()))
    {
        dst.SetSize(rect.getWidth(), rect.getHeight());
    }
    int width = rect.getWidth();
    int height = rect.getHeight();
    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {

            dst(i, j) = src(i + rect.getX(), j + rect.getY());
        }
    }
}
