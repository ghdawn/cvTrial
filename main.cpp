#include <stdio.h>
#include "inc/ImgIO.h"
#include "inc/ImageProcess.h"
#include "inc/GrayBMP.h"
#include "inc/Util.h"
#include "inc/Filter.h"
#include "inc/Vector.h"
#include "inc/Matrix.h"
#include "inc/Draw.h"

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
    GaussianModel model(3, 1.5);
    Filter::Gaussian(gray, gaussian, model);
    int color = 255;
    draw::Circle(gaussian, 200, 200, 40, color);
    draw::LineOffset(gaussian, 300, 200, 0, 100, color);
    draw::LineOffset(gaussian, 300, 200, 100, 0, color);
    draw::LineOffset(gaussian, 300, 200, 100, 100, color);
    draw::LineOffset(gaussian, 300, 200, 0, -100, color);
    draw::LineOffset(gaussian, 300, 200, -100, 0, color);
    draw::LineOffset(gaussian, 300, 200, 100, -100, color);
    draw::Cross(gaussian, 200, 200, 5, color);
    draw::Cross(gaussian, 200, 400, 5, color);
    draw::Cross(gaussian, 400, 200, 5, color);
    draw::Cross(gaussian, 400, 400, 5, color);
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

void testOpticalFlow()
{
    GrayBMP gray1, gray2, grayBig1, grayBig2;
    ImgIO::ReadFromFile("img/bmp/cap000.bmp", grayBig1);
    ImgIO::ReadFromFile("img/bmp/cap003.bmp", grayBig2);
    int scale = 2;
    GaussianModel gSmooth(3, 0.5);
    ImgProcess::DownSampling(grayBig1, gray1, scale);
    ImgProcess::DownSampling(grayBig2, gray2, scale);
    Filter::Gaussian_Applyto(gray1, gSmooth);
    Filter::Gaussian_Applyto(gray2, gSmooth);
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
int main()
{
    // testGaussian();
    // testMatrix();
    // testDiff();
    testScaling();
    // testOpticalFlow();
    // testVector();
    // testOpticalFlow();
    return 0;
}
