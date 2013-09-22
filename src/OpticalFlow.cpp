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
//    char infile[25] = "img/green/cap000.bmp";
    for (int L = 0; L < level; ++L)
    {
        ImgProcess::Dx(I[L], dx[L]);
        ImgProcess::Dy(I[L], dy[L]);
//        sprintf(infile, "dx%d.bmp", L);
//        ImgIO::WriteToFile(dy[L], infile);
//        sprintf(infile, "gray%d.bmp", L);
//        ImgIO::WriteToFile(I[L], infile);
        ixx[L].SetSize(width[L], height[L]);
        ixy[L].SetSize(width[L], height[L]);
        iyy[L].SetSize(width[L], height[L]);
    }
}

int OpticalFlow::Ixx(int L, int x, int y)
{
    if (ixx[L](x, y) > 0)
        return ixx[L](x, y);
    else
    {
        ixx[L](x, y) = dx[L](x, y) * dx[L](x, y);
        return ixx[L](x, y);
    }
}

int OpticalFlow::Ixy(int L, int x, int y)
{
    if (ixy[L](x, y) > 0)
        return ixy[L](x, y);
    else
    {
        ixy[L](x, y) = dx[L](x, y) * dy[L](x, y);
        return ixy[L](x, y);
    }
}

int OpticalFlow::Iyy(int L, int x, int y)
{
    if (iyy[L](x, y) > 0)
        return iyy[L](x, y);
    else
    {
        iyy[L](x, y) = dy[L](x, y) * dy[L](x, y);
        return iyy[L](x, y);
    }
}
void OpticalFlow::Compute(Point& U, Point& V)
{
    //每一层猜测的光流大小
    Vector<float> guess(2);
    Vector<float> v(2), ita(2);
    //每一层的三个微分
    GrayBMP dt;
    //每层待计算的点
    Point uL;
    const int w = 5;
    JACOBI_ARRAY ja(2);
    JACOBI_MATRIX mat(2);
    for (int L = level - 1; L >= 0; --L)
    {
        if (L < (level - 1))
        {
            guess = (guess + v) * 2;
        }
        uL.x = U.x >> L;
        uL.y = U.y >> L;

        v[0] = v[1] = 0;

        int xx = 0, yy = 0, xy = 0;
        for (int j = uL.y - w; j <= uL.y + w; ++j)
        {
            for (int i = uL.x - w; i <= uL.x + w; ++i)
            {
                xx += Ixx(L, i, j);
                yy += Iyy(L, i, j);
                xy += Ixy(L, i, j);
            }
        }

        int xt, yt;
        dt.SetSize(width[L], height[L]);
        float lambda1, lambda2;
        ja[0] = xx;
        ja[1] = xy;
        mat[0] = ja;
        ja[0] = xy;
        ja[1] = yy;
        mat[1] = ja;
        Jacobi jacobi;
        jacobi.setMatrix(mat);
        Vector<float> eig1, eig0;
        lambda1 = jacobi.getEigenValue(0);
        lambda2 = jacobi.getEigenValue(1);
//        jacobi.printEigen();
//        jacobi.printMatrix();
        jacobi.getEigenVec(0, eig0);
        jacobi.getEigenVec(1, eig1);

        if (lambda1 < 0.1)
        {
            v[0] = v[1] = 0;
        }
        else if (lambda1 > 100 * lambda2)
        {
            xt = 0, yt = 0;
            for (int y = uL.y - w; y <= uL.y + w; ++y)
            {
                for (int x = uL.x - w; x <= uL.x + w; ++x)
                {
                    dt(x, y) = J[L](Limit::Round(x + guess[0] + v[0]), Limit::Round(y + guess[1] + v[1]))
                            - I[L](x, y);
                    xt += dt(x, y) * dx[L](x, y);
                    yt += dt(x, y) * dy[L](x, y);
                }
            }
            float n1 = eig0[0] * xt + eig0[1] * yt;
            v[0] = n1 * eig0[0] / lambda1;
            v[1] = n1 * eig0[1] / lambda1;
        }
        else
        {
            //for(int k=0;k<3;++k)
            {
                xt = 0, yt = 0;
                for (int y = uL.y - w; y <= uL.y + w; ++y)
                {
                    for (int x = uL.x - w; x <= uL.x + w; ++x)
                    {
                        dt(x, y) = J[L](Limit::Round(x + guess[0] + v[0]), Limit::Round(y + guess[1] + v[1]))- I[L](x, y);
                        xt += dt(x, y) * dx[L](x, y);
                        yt += dt(x, y) * dy[L](x, y);
                    }
                }
                float n1 = eig0[0] * xt + eig0[1] * yt;
                float n2 = eig1[0] * xt + eig1[1] * yt;
                ita[0] = n1 * eig0[0] / lambda1 + n2 * eig1[0] / lambda2;
                ita[1] = n1 * eig0[1] / lambda1 + n2 * eig1[1] / lambda2;
//            const float det = xx * yy - xy * xy;
//            ita[0] = (xy * yt - yy * xt) / det;
//            ita[1] = (xy * xt - xx * yt) / det;
                v = v + ita;
                printf("%f\n", ita.norm2());
            }// while (ita.norm2() > 0.5);
            printf("Done at :%f\n", ita.norm2());
        }
//        if (v.norm2() > 200)
//        {
//            v[0] = v[1] = 0;
//        }
    }
    V.x = U.x + Limit::Round(guess[0]);
    V.y = U.y + Limit::Round(guess[1]);
}

Rect OpticalFlow::Compute(const Rect& rect)
{
    Rect rectout(rect);
    int x = rect.getX();
    int y = rect.getY();
    int upx = x + rect.getWidth();
    int upy = y + rect.getHeight();
    Point u, v, final;
    int count = (rect.getHeight() * rect.getWidth()) >> 4;
    Point *U = new Point[count];
    Point *V = new Point[count];
    int k = 0;
    for (int j = y; j < upy; j += 4)
    {
        for (int i = x; i < upx; i += 4)
        {
            u.x = i;
            u.y = j;
            Compute(u, v);
            U[k] = u;
            V[k] = v;
            v = v - u;
            final = final + v;
            ++k;
        }
    }

    u.x = u.y = 0;
    for (int i = 0; i < count; ++i)
        u = u + V[i];
    u.x /= count;
    u.y /= count;
//    rectout.setPosition(u.x - (rect.getWidth() >> 1),u.y - (rect.getHeight() >> 1));
    rectout.setPosition(rect.getX() + final.x / count,
            rect.getY() + final.y / count);
    delete[] U;
    delete[] V;
    return rectout;
}

void OpticalFlow::GeneratePyramidal(const GrayBMP& It1, const GrayBMP& It2)
{
    width[0] = It1.getWidth();
    height[0] = It1.getHeight();
    I[0] = It1;
    J[0] = It2;
    for (int L = 1; L < level; L++)
    {
        width[L] = width[L - 1] >> 1;
        height[L] = height[L - 1] >> 1;
        I[L].SetSize(width[L], height[L]);
        J[L].SetSize(width[L], height[L]);
        ImgProcess::Scaling(I[L - 1], I[L]);
        ImgProcess::Scaling(J[L - 1], J[L]);
    }
}

OpticalFlow::OpticalFlow()
{
    level = 3;
}

OpticalFlow::OpticalFlow(int Level)
{
    level = Level;
}
