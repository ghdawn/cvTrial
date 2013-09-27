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
#include "FeaturePoint.h"

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

float OpticalFlow::_minEigenvalue(float gxx, float gxy, float gyy)
{
    return (float) ((gxx + gyy - sqrt((gxx - gyy) * (gxx - gyy) + 4 * gxy * gxy))
            * 0.5f);
}

void OpticalFlow::SelectGoodFeature(const Rect& rect,std::vector<FeaturePoint>& result)
{
    std::vector<FeaturePoint> featurelist(rect.getWidth() * rect.getHeight());
    int upx = rect.getX() + rect.getWidth();
    int upy = rect.getY() + rect.getHeight();
    const int w = 5;
    int k = 0;
    for (int j = rect.getY(); j < upy; ++j)
    {
        for (int i = rect.getX(); i < upx; ++i)
        {
            int xx = 0, yy = 0, xy = 0;
            for (int jj = j - w; jj <= j + w; ++jj)
            {
                for (int ii = i - w; ii <= i + w; ++ii)
                {
                    xx += Ixx(0, ii, jj);
                    yy += Iyy(0, ii, jj);
                    xy += Ixy(0, ii, jj);
                }
            }
            featurelist[k].x = i;
            featurelist[k].y = j;
            featurelist[k].key = _minEigenvalue(xx, xy, yy);
            ++k;
        }
    }
    FeaturePoint max = *max_element(featurelist.begin(), featurelist.end());
    max.key *= 0.05;
    for (vector<FeaturePoint>::iterator i = featurelist.begin();
            i < featurelist.end(); ++i)
        if (i->key < max.key)
            i->key = -1;
    sort(featurelist.begin(), featurelist.end(), greater<FeaturePoint>());
    FeaturePoint feat;
    for (int i = 0; i < featurelist.size(); ++i)
    {
        //printf("%f ",featurelist[i].key);
        if (featurelist[i].key > 0)
        {
            feat = featurelist[i];
            k=0;
            for (int j = i + 1; j < featurelist.size(); ++j)
            {
                if ((Math::Abs(featurelist[j].x - feat.x) <= 5)
                        && (Math::Abs(featurelist[j].y - feat.y) <= 5))
                {
                    featurelist[j].key = -1;
                    ++k;
                }
                if(k>=25)break;
            }
        }
    }
    k = 0;
    for (int i = 0; i < featurelist.size(); ++i)
    {
        if (featurelist[i].key > 0)
        {
            result[k] = featurelist[i];
            ++k;
        }
        if (k >= result.size())
            break;
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
void OpticalFlow::Compute(Point& U, Point& V, bool Forward)
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
                    if (Forward)
                    {
                        dt(x, y) = I[L](x, y)
                                - ImgProcess::Interpolation(J[L],
                                        x + guess[0] + v[0],
                                        y + guess[1] + v[1]);
                    }
                    else
                    {
                        dt(x, y) = J[L](x, y)
                                - ImgProcess::Interpolation(I[L],
                                        x + guess[0] + v[0],
                                        y + guess[1] + v[1]);
                    }
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
//            for (int k = 0; k < 3; ++k)
            {
                xt = 0, yt = 0;
                for (int y = uL.y - w; y <= uL.y + w; ++y)
                {
                    for (int x = uL.x - w; x <= uL.x + w; ++x)
                    {
                        if (Forward)
                        {
                            dt(x, y) = I[L](x, y)
                                    - ImgProcess::Interpolation(J[L],
                                            x + guess[0] + v[0],
                                            y + guess[1] + v[1]);
                        }
                        else
                        {
                            dt(x, y) = J[L](x, y)
                                    - ImgProcess::Interpolation(I[L],
                                            x + guess[0] + v[0],
                                            y + guess[1] + v[1]);
                        }
                        xt += dt(x, y) * dx[L](x, y);
                        yt += dt(x, y) * dy[L](x, y);
                    }
                }
                float n1 = eig0[0] * xt + eig0[1] * yt;
                float n2 = eig1[0] * xt + eig1[1] * yt;
                ita[0] = n1 * eig0[0] / lambda1 + n2 * eig1[0] / lambda2;
                ita[1] = n1 * eig0[1] / lambda1 + n2 * eig1[1] / lambda2;
//                const float det = xx * yy - xy * xy;
//                ita[0] = (xy * yt - yy * xt) / det;
//                ita[1] = (xy * xt - xx * yt) / det;
                v = v + ita;
//                printf("%f\n", ita.norm2());
            }
//            printf("Done at :%f\n", ita.norm2());
        }
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
    int count = (rect.getHeight() * rect.getWidth()); // >> 2;
    Point *U = new Point[count];
    Point *V = new Point[count];
    float *error = new float[count];
    int k = 0;
    vector<FeaturePoint> feat(100);
    SelectGoodFeature(rect,feat);
//    for (int j = y; j < upy; j += 2)
//        for (int i = x; i < upx; i += 2)
    for(vector<FeaturePoint>::iterator i=feat.begin();i<feat.end();++i)
        {
            u.x = i->x;
            u.y = i->y;
            Compute(u, v, true);
            U[k] = u;
            V[k] = v;
            ++k;
        }

    u.x = u.y = 0;
    v.x = v.y = 0;
    Vector<float> ita(2);
    for (int i = 0; i < k; ++i)
    {
        Compute(V[i], u, false);
        ita[0] = u.x - U[i].x;
        ita[1] = u.y - U[i].y;
        error[i] = ita.norm2();
    }
    for (int i = 1; i < k; i++)
    {
        if (error[i - 1] > error[i])
        {
            float temp = error[i];
            v = V[i];
            u = U[i];
            int j = i;
            while (j > 0 && error[j - 1] > temp)
            {
                error[j] = error[j - 1];
                V[j] = V[j - 1];
                U[j] = U[j - 1];
                j--;
            }
            error[j] = temp;
            U[j] = u;
            V[j] = v;
        }
    }
    u.x = u.y = 0;
    v.x = v.y = 0;
    k = k>>1;
    for (int i = 0; i < k; ++i)
    {
        u = u + U[i];
        v = v + V[i];
    }
    u.x /= k;
    u.y /= k;
    v.x /= k;
    v.y /= k;
//    rectout.setPosition(u.x - (rect.getWidth() >> 1),u.y - (rect.getHeight() >> 1));
    rectout.setPosition(rect.getX() + v.x - u.x, rect.getY() + v.y - u.y);
    delete[] U;
    delete[] V;
    delete[] error;
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
