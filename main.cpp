#include <iostream>
#include "EasyBMP.h"
#include "ImageProcess.h"
#include "GrayBMP.h"
#include "Util.cpp"
using namespace std;

void testDiff()
{
	BMP Input;
	GrayBMP gray;
	Input.ReadFromFile("img/table1.bmp");
	gray.SetSize(Input.TellWidth(),Input.TellHeight());
	ConvertToGray(Input,gray);
	WriteToFile(gray,"gray.bmp");
	GrayBMP test(gray);
	Dx(gray,test);
	WriteToFile(test,"dx.bmp");
	Dy(gray,test);
	WriteToFile(test,"Dy.bmp");
	Sobel(gray,test);
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
	testGaussian();
	testDiff();
	return 0;
}