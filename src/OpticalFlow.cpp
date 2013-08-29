/*
 * OpticalFlow.cpp
 *
 *  Created on: 2013-8-29
 *      Author: ghdawn
 */

#include "OpticalFlow.h"
#include "GrayBMP.h"

void LK_Method(GrayBMP gray1, const GrayBMP& gray2)
{
    int width = gray1.getWidth();
    int height = gray1.getHeight();
    GrayBMP dx(width, height), dy(width, height), dt(width, height);
    ImgProcess::Dx(gray1, dx);
    ImgProcess::Dy(gray1, dy);
    dt = gray2 - gray1;
    ImgIO::WriteToFile(dt, "Dt.bmp");
    GrayBMP Ixx(width, height), Iyy(width, height), Ixy(width, height), Ixt(
            width, height), Iyt(width, height);
    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            const float ix = dx(i, j);
            const float iy = dy(i, j);
            const float it = dt(i, j);
            Ixx(i, j) = ix * ix;
            Iyy(i, j) = iy * iy;
            Ixy(i, j) = ix * iy;
            Ixt(i, j) = ix * it;
            Iyt(i, j) = iy * it;
        }
    }
    GaussianModel gCov(5, 1);
    Filter::Gaussian_Applyto(Ixx, gCov);
    Filter::Gaussian_Applyto(Ixy, gCov);
    Filter::Gaussian_Applyto(Iyy, gCov);
    Filter::Gaussian_Applyto(Ixt, gCov);
    Filter::Gaussian_Applyto(Iyt, gCov);
    Vector<float> u(2);
    int range = 3;
    for (int i = 0; i < width; i += range)
        for (int j = 0; j < height; j += range)
        {
            const float xx = Ixx(i, j);
            const float yy = Iyy(i, j);
            const float xy = Ixy(i, j);
            const float det = xx * yy - xy * xy;
            if (Math::Abs(det) < 20)
            {
                u[0] = u[1] = 0;
            }
            else
            {
                const float xt = Ixt(i, j);
                const float yt = Iyt(i, j);
                u[0] = (xy * yt - yy * xt) / det;
                u[1] = (xy * xt - xx * yt) / det;
                int color = 255;
                // printf("X:(%d,%d)\nU:(%d,%d)\n",i,j,static_cast<int>(u[0]),static_cast<int>(u[1]));
                draw::LineOffset(gray1, i, j, static_cast<int>(u[0]),
                        static_cast<int>(u[1]), color);
                // draw::Cross(gray1,i+static_cast<int>(u[0]),j+static_cast<int>(u[1]),3);
            }
        }
    ImgIO::WriteToFile(gray1, "of.bmp");
}
