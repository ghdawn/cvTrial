#include <iostream>
#include "EasyBMP.h"
#include "ImageProcess.h"

using namespace std;

void testDiff()
{
	BMP test,gray;
	test.ReadFromFile("img/table1.bmp");
	gray.SetSize(test.TellWidth(),test.TellHeight());
	gray.SetBitDepth(8);
	ConvertToGray(test,gray);
	test.SetBitDepth(8);
	CreateGrayscaleColorTable( test );
	Dx(gray,test);
	test.WriteToFile("dx.bmp");
	Dy(gray,test);
	test.WriteToFile("Dy.bmp");
	Sobel(gray,test);
	test.WriteToFile("Sobel.bmp");

}

void testGaussian()
{
	const double pi=3.1415926;
	double a[5][5];
	double var=1;
	for (int i = 0; i < 5; ++i)
	{
		for(int j=0; j < 5; ++j)
		{
			a[i][j]=exp(-(Square(i-2)+Square(j-2))/(2*var*var))/(2*pi*var*var);
			printf("%lf  ",a[i][j] );
		}
		printf("\n");
	}


}
int main()
{
	testGaussian();
	
	return 0;
}