#include <iostream>
#include "EasyBMP.h"
#include "ImageProcess.h"
#include "GrayBMP.h"
#include "Util.cpp"
#include "Filter.h"
#include "Vector.h"
#include "Matrix.h"
using namespace std;

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
		v[i]=1;
	}
	printVec(v);
	Vector<Byte> b;
	b=v;
    printVec(b);
	printf("%d %d\n", v*b,b*v);
	printVec(v);
	printVec(b);
}

void printBMP(const GrayBMP& test)
{
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			printf("%3d ", test(i,j));
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
	gray.SetSize(Input.TellWidth(),Input.TellHeight());
	ConvertToGray(Input,gray);
	WriteToFile(gray,"gray.bmp");
	GrayBMP test(gray);
	GrayBMP gaussian(gray);

	//printBMP(gray);
	Filter_Gaussian(gray,gaussian,3,1.5);
	WriteToFile(gaussian,"Filter_Gaussian.bmp");
	Dx(gaussian,test);
	//printBMP(test);
	WriteToFile(test,"dx.bmp");

	Dy(gaussian,test);
	//printBMP(test);
	WriteToFile(test,"Dy.bmp");

	Sobel(gaussian,test);
	//printBMP(test);
	WriteToFile(test,"Sobel.bmp");
}


void testGaussian()
{
	
	double a[5][5];
	double var=1;
	for (int i = 0; i < 5; ++i)
	{
		for(int j=0; j < 5; ++j)
		{
			a[i][j]=Math::Gaussian(i-2,j-2,var);
			printf("%lf  ",a[i][j] );
		}
		printf("\n");
	}
}

void testOpticalFlow()
{

}

void testMatrix()
{
	float data[]={1,1,1,-1};
	Matrix<float> mat(2,2,data);
	mat.printMatrix();
	Matrix<float> mat2=mat.inv();
	mat2.printMatrix();
	Vector<float> result(2),b(2);
	b[0]=2;
	b[1]=1;
	result=mat2*b;
	printVec(result);
}
int main()
{
	testMatrix();
	// testVector();
	// testGaussian();
	// testDiff();
	// testOpticalFlow();

	return 0;
}