#ifndef __IMAGE_PROCESS_H
#define __IMAGE_PROCESS_H

class BMP;
class GrayBMP;
class Rect;
template<typename T>
class Vector;
class ImgProcess
{
    public:
        static void Dx(const GrayBMP &src, GrayBMP &dx);
        static void Dy(const GrayBMP &src, GrayBMP &dx);
        static void Sobel(const GrayBMP &src, GrayBMP &dx);
        static void DownSampling(const GrayBMP & bmp, GrayBMP& result, int scale);
        static void Scaling(const GrayBMP& src, GrayBMP& dst);
        static Vector<float> GetSquare(const GrayBMP& src, int x, int y, int Range);
        static Vector<float> toVector(const GrayBMP& src);
        static void GetSquare(const GrayBMP& src,GrayBMP& dst,Rect rect);
};
#endif
