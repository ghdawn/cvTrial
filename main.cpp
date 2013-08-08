#include <iostream>
#include "EasyBMP.h"
#include "ImageProcess.h"
#include "GrayBMP.h"
#include "Util.cpp"
#include "Filter.h"
#include "Vector.h"
using namespace std;

void printVec(Vector<Byte> &v)
{
	for (int i = 0; i < 10; ++i)
	{
		printf("%d ", v[i]);
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
	// Filter_Median(gray,test,5);
	// WriteToFile(test,"Filter_Median.bmp");
	Filter_Gaussian(gray,gaussian,7,1.5);
	WriteToFile(gaussian,"Filter_Gaussian.bmp");
	Dx(gaussian,test);
	WriteToFile(test,"dx.bmp");
	Dy(gaussian,test);
	WriteToFile(test,"Dy.bmp");
	Sobel(gaussian,test);
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
int main()
{
	// testVector();
	// testGaussian();
	testDiff();
	return 0;
}