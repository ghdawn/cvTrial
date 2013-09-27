#ifndef __IMAGE_PROCESS_H
#define __IMAGE_PROCESS_H

#include "GrayBMP.h"
class BMP;
class Rect;
template<typename T>
class Vector;
class ImgProcess
{
    public:

        static void Dx(const GrayBMP &src, GrayBMP &dx);
        static void Dy(const GrayBMP &src, GrayBMP &dx);
        static void Sobel(const GrayBMP &src, GrayBMP &dx);
        static void DownSampling(const GrayBMP & bmp, GrayBMP& result,
                int scale);
        static void Scaling(const GrayBMP& src, GrayBMP& dst);
        static Vector<float> GetSquare(const GrayBMP& src, int x, int y,
                int Range);
        static Vector<float> toVector(const GrayBMP& src);
        static void GetSquare(const GrayBMP& src, GrayBMP& dst, Rect rect);
        static inline int Interpolation(const GrayBMP& src, float x, float y)
        {
            const int FACTOR = 2048;
            const int BITS = 22;
            int x0, y0;
            int u, v, u_1, v_1;
            x0 = (int) (x);
            y0 = (int) (y);
            u = (x - x0) * FACTOR;
            v = (y - y0) * FACTOR;
            u_1 = FACTOR - u;
            v_1 = FACTOR - v;
            int result = (src(x0, y0) * u_1 + src(x0 + 1, y0) * u) * v_1
                    + (src(x0, y0 + 1) * u_1 + src(x0 + 1, y0 + 1) * u) * v;
            return result >> BITS;
        }
};
#endif
