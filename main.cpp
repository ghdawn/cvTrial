#include <stdio.h>
#include "inc/ImgIO.h"
#include "inc/ImageProcess.h"
#include "inc/GrayBMP.h"
#include "inc/Util.h"
#include "inc/Filter.h"
#include "inc/Vector.h"
#include "inc/Matrix.h"
#include "inc/Draw.h"
#include "inc/Structure/Rect.h"
#include "inc/Structure/BasicStruct.h"
#include "inc/Detector.h"
#include "inc/Jacobi.h"
#include "inc/OpticalFlow.h"
#include "stdlib.h"
#include <iostream>
using std::cout;
using std::endl;

void printVec(Vector<Byte> &v)
{
    for (int i = 0; i < 10; ++i)
    {
        printf("%d ", v[i]);
    }
    printf("\n");
}

void printVec(Vector<float> &v)
{
    for (int i = 0; i < v.dim(); ++i)
    {
        printf("%f ", v[i]);
    }
    printf("\n");
}
void testVector()
{
    Vector<Byte> v(10);
    for (int i = 0; i < 10; ++i)
    {
        v[i] = 1;
    }
    printVec(v);
    Vector<Byte> b;
    b = v;
    printVec(b);
    printf("%d %d\n", v * b, b * v);
    printVec(v);
    printVec(b);
}

void printBMP(const GrayBMP& test)
{
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            printf("%3d ", test(i, j));
        }
        printf("\n");
    }
    printf("\n");
}

void testDiff()
{
    GrayBMP gray;
    ImgIO::ReadFromFile("img/table1.bmp", gray);
    ImgIO::WriteToFile(gray, "gray.bmp");
    GrayBMP test(gray);
    GrayBMP gaussian(gray);

    // printBMP(gray);
    int t[] =
    { 1, 4, 6, 4, 1 };
    GaussianModel model(3, 1.5);
    int begin = clock() / 1000, end;
    //for (int i = 0; i < 100; i++)
    Filter::Gaussian(gray, gaussian, model);
    end = clock() / 1000;
    printf("Style1: %d\n", end - begin);
    ImgIO::WriteToFile(gaussian, "gau1.bmp");
    begin = clock() / 1000;
    //for (int i = 0; i < 100; i++)
    Filter::Conv(gray, gaussian, t, 5);
    end = clock() / 1000;
    printf("Style2: %d\n", end - begin);

    ImgIO::WriteToFile(gaussian, "gau2.bmp");

//	WriteToFile(gaussian, "Filter_Gaussian.bmp");
    ImgProcess::Dx(gaussian, test);
    // printBMP(test);
    ImgIO::WriteToFile(test, "dx.bmp");

    ImgProcess::Dy(gaussian, test);
    // printBMP(test);
    ImgIO::WriteToFile(test, "dy.bmp");

    ImgProcess::Sobel(gaussian, test);
    // printBMP(test);
    ImgIO::WriteToFile(test, "Sobel.bmp");
}

void testGaussian()
{
    double Var = 3;
    int Range = 5;
    int mid = 2;
    Vector<float> gaussian(Range * Range);
    int k = 0;
    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            gaussian[k++] = Math::Gaussian(i - mid, j - mid, Var);
            printf("%lf  ", gaussian[k - 1]);
        }
        printf("\n");
    }
    printf("%f\n", gaussian.sum());
    getchar();
}

void testEigen()
{
    JACOBI_ARRAY ja(3), value;
    JACOBI_MATRIX jm, vector;
    for (int i = 0; i < 100; ++i)
    {
        jm.clear();
        ja[0] = 1;
        ja[1] = -1;
        ja[2] = 0;
        jm.push_back(ja);
        ja[0] = -1;
        ja[1] = 1;
        ja[2] = 2;
        jm.push_back(ja);
        ja[0] = 0;
        ja[1] = 2;
        ja[0] = 1;
        jm.push_back(ja);
        Jacobi j;
        j.setMatrix(jm);
        value = j.getEigenvalues();
        vector = j.getEigenvectors();
        j.printEigen();
    }
}
void testLKPy()
{
    GrayBMP gray1, gray2, grayBig1, grayBig2;
    char infile[25] = "img/green/cap000.bmp";
    char outfile[25] = "img/out/cap000.bmp";
    ImgIO::ReadFromFile("img/green/cap000.bmp", grayBig1);
    ImgIO::ReadFromFile("img/table1.bmp", grayBig1);
    ImgIO::ReadFromFile("img/table2.bmp", grayBig2);
    int scale = 2;

    ImgProcess::DownSampling(grayBig1, gray1, scale);
    ImgProcess::DownSampling(grayBig2, gray2, scale);
    int t[] =
    { 1, 4, 6, 4, 1 };
    Filter::ConvApplyto(gray1, t, 5);
    OpticalFlow flow;

    Point u, v, final;
    final.x = final.y = 0;
    //flow.Init(gray1, gray2);
//    Rect rect(20, 180,40, 40);
//    Rect rect(103, 122,60, 60);
    Rect rect(10, 10, 300, 230);
    //gray2=gray1;
    //int count=1;
    //for (int k = 0; k < 100; ++k)
    {
        // gray1=gray2;
//        sprintf(infile,"img/green/cap%03d.bmp",k+1);
//        ImgIO::ReadFromFile(infile, grayBig2);
//        ImgProcess::DownSampling(grayBig2, gray2, scale);
//        Filter::ConvApplyto(gray2, t, 5);
        int begin = clock() / 1000;
        int x = rect.getX();
        int y = rect.getY();
        int upx = x + rect.getWidth();
        int upy = y + rect.getHeight();
        final.x = final.y = 0;
        flow.Init(gray1, gray2);
        for (int j = y; j < upy; j += 4)
        {
            for (int i = x; i < upx; i += 4)
            {
                u.x = i;
                u.y = j;
                flow.Compute(u, v);
                v = v - u;
                printf("(%d,%d)  ", v.x, v.y);
                final = final + v;
//                if(v.x!=0 && v.y!=0)
//                    ++count;
                draw::LineOffset(gray1, i, j, v.x, v.y, 255);
            }
        }
        //rect.setPosition(rect.getX()+final.x/count,rect.getY()+final.y/count);
        //rect=flow.Compute(rect);
//        printf("\nNo.%d\n%d %d\n",k,rect.getX(),rect.getY());

        draw::Rectangle(gray1, rect, 255);
//        sprintf(outfile,"img/out/cap%03d.bmp",k);
        ImgIO::WriteToFile(gray1, outfile);
        int end = clock() / 1000;
        printf("time:%d\n", end - begin);
        cout << endl;
    }
}

void testOpticalFlow()
{
    GrayBMP gray1, gray2, grayBig1, grayBig2;
    ImgIO::ReadFromFile("img/table1.bmp", grayBig1);
    ImgIO::ReadFromFile("img/table2.bmp", grayBig2);
    int scale = 2;
    GaussianModel gSmooth(3, 0.5);
    ImgProcess::DownSampling(grayBig1, gray1, scale);
    ImgProcess::DownSampling(grayBig2, gray2, scale);
    Filter::GaussianApplyto(gray1, gSmooth);
    Filter::GaussianApplyto(gray2, gSmooth);
    ImgIO::WriteToFile(grayBig1, "grayBig1.bmp");
    ImgIO::WriteToFile(gray1, "gray1.bmp");

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
            const int ix = dx(i, j);
            const int iy = dy(i, j);
            const int it = dt(i, j);
            Ixx(i, j) = ix * ix;
            Iyy(i, j) = iy * iy;
            Ixy(i, j) = ix * iy;
            Ixt(i, j) = ix * it;
            Iyt(i, j) = iy * it;
        }
    }
    GaussianModel gCov(15, 1);
    Filter::GaussianApplyto(Ixx, gCov);
    Filter::GaussianApplyto(Ixy, gCov);
    Filter::GaussianApplyto(Iyy, gCov);
    Filter::GaussianApplyto(Ixt, gCov);
    Filter::GaussianApplyto(Iyt, gCov);
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

void testMatrix()
{
    float data[] =
    { 1, 1, 1, -1 };
    Matrix<float> mat(2, 2, data);
    mat.printMatrix();
    Matrix<float> mat2 = mat.inv();
    mat2.printMatrix();
    Vector<float> result(2), b(2);
    b[0] = 2;
    b[1] = 1;
    result = mat2 * b;
    printVec(result);
}

void testScaling()
{
    GrayBMP gray;
    ImgIO::ReadFromFile("img/table1.bmp", gray);
    ImgIO::WriteToFile(gray, "gray.bmp");
    GrayBMP test(480, 640);
    ImgProcess::Scaling(gray, test);
    ImgIO::WriteToFile(test, "scal.bmp");

}

void testDetect()
{
    GrayBMP bmp, gray, patch;
    char filename[25] = "img/bmp/cap000.bmp";
    ImgIO::ReadFromFile(filename, bmp);
    ImgProcess::DownSampling(bmp, gray, 2);
    Rect rect(15, 194, 40, 40);
    ImgProcess::GetSquare(gray, patch, rect);
    ImgIO::WriteToFile(patch, "Patch.bmp");
    Detector detector(patch);
    for (int i = 1; i < 101; i++)
    {
        sprintf(filename, "img/bmp/cap%03d.bmp", i);
        ImgIO::ReadFromFile(filename, bmp);
        ImgProcess::DownSampling(bmp, gray, 2);
        rect = detector.MatchPatch(gray);
        draw::Rectangle(gray, rect, 255);
        sprintf(filename, "img/out/cap%03d.bmp", i);
        ImgIO::WriteToFile(gray, filename);
    }
}
int main()
{
    // testGaussian();
    // testMatrix();
//    testDiff();
//    testScaling();
//    testOpticalFlow();
    testLKPy();
//    testEigen();
//    testDetect();
    // testVector();
//     testOpticalFlow();
    return 0;
}
