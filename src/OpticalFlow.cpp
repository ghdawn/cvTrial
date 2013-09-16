/*
 * OpticalFlow.cpp
 *
 *  Created on: 2013-9-11
 *      Author: ghdawn
 */
#include "GrayBMP.h"
#include "ImgIO.h"
#include "ImageProcess.h"
#include "Structure/Rect.h"
#include "Structure/BasicStruct.h"
#include "stdio.h"
#include "Draw.h"
#include "Vector.h"
#include "Jacobi.h"
#include "OpticalFlow.h"

void OpticalFlow::Init(const GrayBMP& It1, const GrayBMP& It2)
{
    GeneratePyramidal(It1, It2);
}

void OpticalFlow::Compute(Point& U, Point& V)
{
    char file[20];
    //每一层猜测的光流大小
    Vector<float> guess(2);
    Vector<float> v(2), ita(2);
    //每一层的三个微分
    GrayBMP dx, dy, dt;
    //三个微分对应的乘积
    GrayBMP Ixx, Iyy, Ixy, Ixt, Iyt;
    //每层待计算的点
    Point uL;
    int width, height;
    const float threshold = 0.05;
    const int w = 5;

    for (int L = 2; L >= 0; --L)
    {
        width = I[L].getWidth();
        height = J[L].getHeight();

        uL.x = U.x >> L;
        uL.y = U.y >> L;
        ImgProcess::Dx(I[L], dx);
        ImgProcess::Dy(I[L], dy);
        Ixx.SetSize(width, height);
        Iyy.SetSize(width, height);
        Ixy.SetSize(width, height);
        Ixt.SetSize(width, height);
        Iyt.SetSize(width, height);
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                const int ix = dx(i, j);
                const int iy = dy(i, j);
                Ixx(i, j) = ix * ix;
                Iyy(i, j) = iy * iy;
                Ixy(i, j) = ix * iy;
            }
        }

        v[0] = v[1] = 0;

        int xx = 0, yy = 0, xy = 0;
        for (int j = uL.y - w; j <= uL.y + w; ++j)
            for (int i = uL.x - w; i <= uL.x + w; ++i)
            {
                const int ix = dx(i, j);
                const int iy = dy(i, j);
                Ixx(i, j) = ix * ix;
                Iyy(i, j) = iy * iy;
                Ixy(i, j) = ix * iy;
                xx += Ixx(i, j);
                yy += Iyy(i, j);
                xy += Ixy(i, j);
            }
        JACOBI_MATRIXmatrix;
        matrix[0][0] = xx;
        matrix[0][1] = matrix[1][0] = xy;
        matrix[1][1] = yy;
        Jacobi jacobi;
        jacobi.setMatrix(matrix);
        const float det = xx * yy - xy * xy;
        int xt, yt;
        Vector<float> alpha1, alpha2;
        dt.SetSize(dx.getWidth(), dx.getHeight());
        float lambda1 = jacobi.getEigenValue(0), lambda2 = jacobi.getEigenValue(
                1);
//        float a = (xx + yy), b = sqrt(Math::Square(xx - yy) + 4 * xy * xy);
//        lambda1 = a + b;
//        lambda2 = a - b;

        for (int k = 0; k < 1; k++)
        {
            if (lambda1 < 0.1)
            {
                v[0] = v[1] = 0;
            }
            else if (lambda1 > 30 * lambda2)
            {
                jacobi.getEigenVec(0, alpha1);
                for (int y = uL.y - w; y <= uL.y + w; ++y)
                {
                    for (int x = uL.x - w; x <= uL.x + w; ++x)
                    {
                        dt(x, y) = I[L](x, y)
                                - J[L](x + guess[0] + v[0],
                                        y + guess[1] + v[1]);
                        xt += dt(x, y) * dx(x, y);
                        yt += dt(x, y) * dy(x, y);
                    }
                }
                float n2 = alpha1[0] * xt+alpha1[1*yt];
                v[0]=n2*alpha1[0]/lambda1;
                v[1]=n2*alpha1[1]/lambda1;
            }
            else
            {
                xt = 0, yt = 0;
                for (int y = uL.y - w; y <= uL.y + w; ++y)
                {
                    for (int x = uL.x - w; x <= uL.x + w; ++x)
                    {
                        dt(x, y) = I[L](x, y)
                                - J[L](x + guess[0] + v[0],
                                        y + guess[1] + v[1]);
                        xt += dt(x, y) * dx(x, y);
                        yt += dt(x, y) * dy(x, y);
                    }
                }
                ita[0] = (xy * yt - yy * xt) / det;
                ita[1] = (xy * xt - xx * yt) / det;

                v = v + ita;
            }
        }
        //while (ita.norm1() > threshold);

        guess = (guess + v) * 2;
        sprintf(file, "pyramidal%d.bmp", L);
        ImgIO::WriteToFile(I[L], file);
    }
    V.x = U.x + guess[0] * 0.5;
    V.y = U.y + guess[1] * 0.5;
}

void OpticalFlow::Compute(GrayBMP& u, GrayBMP& v)
{
}

void OpticalFlow::Compute(GrayBMP& u, GrayBMP& v, Rect rect)
{
}

void OpticalFlow::GeneratePyramidal(const GrayBMP& It1, const GrayBMP& It2)
{
    width[0] = It1.getWidth();
    height[0] = It1.getHeight();
    I[0] = It1;
    J[0] = It2;
    for (int L = 1; L < 3; L++)
    {
        width[L] = width[L - 1] >> 1;
        height[L] = height[L - 1] >> 1;
        I[L].SetSize(width[L], height[L]);
        J[L].SetSize(width[L], height[L]);
        ImgProcess::Scaling(I[L - 1], I[L]);
        ImgProcess::Scaling(J[L - 1], J[L]);
    }
}

