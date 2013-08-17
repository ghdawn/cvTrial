#include <stdio.h>
#include "inc/EasyBMP.h"
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
    BMP Input;
    GrayBMP gray;
    Input.ReadFromFile("img/table1.bmp");
    gray.SetSize(Input.TellWidth(), Input.TellHeight());
    ConvertToGray(Input, gray);
    WriteToFile(gray, "gray.bmp");
    GrayBMP test(gray);
    GrayBMP gaussian(gray);

    // printBMP(gray);
    GaussianModel model(3,1.5);
    Filter_Gaussian(gray, gaussian, model);
    draw::Circle(gaussian, 200, 200, 40);
    draw::LineOffset(gaussian, 300, 200, 0, 100);
    draw::LineOffset(gaussian, 300, 200, 100, 0);
    draw::LineOffset(gaussian, 300, 200, 100, 100);
    draw::LineOffset(gaussian, 300, 200, 0, -100);
    draw::LineOffset(gaussian, 300, 200, -100, 0);
    draw::LineOffset(gaussian, 300, 200, 100, -100);
    draw::Cross(gaussian, 200, 200, 5);
    draw::Cross(gaussian, 200, 400, 5);
    draw::Cross(gaussian, 400, 200, 5);
    draw::Cross(gaussian, 400, 400, 5);
    WriteToFile(gaussian, "Filter_Gaussian.bmp");
    Dx(gaussian, test);
    // printBMP(test);
    WriteToFile(test, "dx.bmp");

    Dy(gaussian, test);
    // printBMP(test);
    WriteToFile(test, "Dy.bmp");

    Sobel(gaussian, test);
    // printBMP(test);
    WriteToFile(test, "Sobel.bmp");
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
    BMP Input1, Input2;
    GrayBMP gray1, gray2,grayBig1,grayBig2;
    Input1.ReadFromFile("img/table1.bmp");
    Input2.ReadFromFile("img/table2.bmp");
    gray1.SetSize(Input1.TellWidth()/2, Input1.TellHeight()/2);
    gray2.SetSize(Input1.TellWidth()/2, Input1.TellHeight()/2);
    grayBig1.SetSize(Input1.TellWidth(), Input1.TellHeight());
    grayBig2.SetSize(Input1.TellWidth(), Input1.TellHeight());
    ConvertToGray(Input1, grayBig1);
    ConvertToGray(Input2, grayBig2);
    DownSampling(grayBig1,gray1, 2);
    DownSampling(grayBig1,gray2, 2);
    int width = gray1.TellWidth();
    int height = gray1.TellHeight();
    GrayBMP dx(width, height), dy(width, height), dt(width, height);
    Dx(gray1, dx);
    Dy(gray1, dy);
    dt = gray2 - gray1;
    WriteToFile(dt, "Dt.bmp");
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
    GaussianModel gaussian(5,3);
    Filter_Gaussian_Applyto(Ixx,gaussian);
    Filter_Gaussian_Applyto(Ixy,gaussian);
    Filter_Gaussian_Applyto(Iyy,gaussian);
    Filter_Gaussian_Applyto(Ixt,gaussian);
    Filter_Gaussian_Applyto(Iyt,gaussian);
    Vector<float> u(2);
    for (int i = 0; i < width; i += 5)
        for (int j = 0; j < height; j += 5)
        {
            const float xx = Ixx(i, j);
            const float yy = Iyy(i, j);
            const float xy = Ixy(i, j);
            const float det = xx * yy - xy * xy;

            if (Math::Abs(det) < 0.1)
            {
                u[0] = u[1] = 0;
            }
            else
            {
                const float xt = Ixt(i, j);
                const float yt = Iyt(i, j);
                u[0] = (yy * xt - xy * yt) / det;
                u[1] = (xx * yt - xy * xt) / det;
                // printf("X:(%d,%d)\nU:(%d,%d)\n",i,j,static_cast<int>(u[0]),static_cast<int>(u[1]));
                draw::LineOffset(gray1, i, j, static_cast<int>(u[0]), static_cast<int>(u[1]));
                // draw::Cross(gray1,i+static_cast<int>(u[0]),j+static_cast<int>(u[1]),3);
            }
        }
    WriteToFile(gray1, "of.bmp");
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
int main()
{
    // testGaussian();
    // testMatrix();
    // testDiff();
    testOpticalFlow();
    // testVector();
    // testOpticalFlow();
    return 0;
}
