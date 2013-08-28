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
    for (int i = 0; i < src.getWidth(); ++i)
    {
        for (int j = 0; j < src.getHeight(); ++j)
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
                dx(i, j) = Limit::GrayByte(TempBYTE / 8);
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
    for (int i = 0; i < src.getWidth(); ++i)
    {
        for (int j = 0; j < src.getHeight(); ++j)
        {
            if ((i == 0) || (j == 0) || (i == (src.getWidth() - 1))
                    || (j == (src.getHeight() - 1)))
            {
                dy(i, j) = src(i, j);
            }
            else
            {
                int TempBYTE = (src(i - 1, j - 1) - src(i - 1, j + 1))
                        + 2 * (src(i, j - 1) - src(i, j + 1))
                        + (src(i + 1, j - 1) - src(i + 1, j + 1));
                //int TempBYTE = Limit::Round((src(i, j + 1) - src(i, j - 1)) / 2.0);
                dy(i, j) = Limit::GrayByte(TempBYTE / 8);
            }
        }
    }
}

void ImgProcess::Sobel(const GrayBMP &src, GrayBMP &result)
{
    if (result.MatchSize(src.getWidth(), src.getHeight()))
    {
        result.SetSize(src.getWidth(), src.getHeight());
    }
    for (int i = 0; i < src.getWidth(); ++i)
    {
        for (int j = 0; j < src.getHeight(); ++j)
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
                        + (src(i + 1, j + 1) - src(i - 1, j + 1)) / 8;
                int dy = (src(i - 1, j - 1) - src(i - 1, j + 1))
                        + 2 * (src(i, j - 1) - src(i, j + 1))
                        + (src(i + 1, j - 1) - src(i + 1, j + 1)) / 8;
                result(i, j) = Limit::GrayByte(Math::Abs(dx) + Math::Abs(dy));
            }
        }
    }
}

void ImgProcess::DownSampling(const GrayBMP & bmp, GrayBMP& result, int scale)
{
    if (result.MatchSize(bmp.getWidth() / scale, bmp.getHeight() / scale))
    {
        result.SetSize(bmp.getWidth() / scale, bmp.getHeight() / scale);
    }
    for (int i = 0; i < result.getWidth(); ++i)
    {
        for (int j = 0; j < result.getHeight(); ++j)
        {
            result(i, j) = bmp(i * scale, j * scale);
        }
    }
}

void ImgProcess::Scaling(const GrayBMP& src, GrayBMP& dst)
{
    const int FACTOR = 2048;
    const int BITS = 22;
    int width = dst.getWidth(), height = dst.getHeight();
    float fw = float(src.getWidth()) / width;
    float fh = float(src.getHeight()) / height;
    float x, y;
    int x0, y0, x1, y1;
    int u, v, u_1, v_1;
    int result;
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++)
        {
            x = i * fw;
            y = j * fh;
            x0 = (int) x;
            y0 = (int) y;
            u = (x - x0) * FACTOR;
            v = (y - y0) * FACTOR;
            u_1 = FACTOR - u;
            v_1 = FACTOR - v;
            result = (src(x0, y0) * u_1 + src(x0 + 1, y0) * u) * v_1
                    + (src(x0, y0 + 1) * u_1 + src(x0 + 1, y0 + 1) * u) * v;
            dst(i, j) = result >> BITS;
        }
}

Vector<float> ImgProcess::GetSquare(const GrayBMP& src, int x, int y, int Range)
{
    Vector<float> vec(Range * Range);
    int k = 0;
    Range /= 2;
    for (int i = x - Range; i <= x + Range; ++i)
    {
        for (int j = y - Range; j <= y + Range; ++j)
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
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            dst(i, j) = src(i + rect.getX(), j + rect.getY());
        }
    }
}
