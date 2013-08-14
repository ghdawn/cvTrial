#include <iostream>
#include "EasyBMP.h"
#include "ImageProcess.h"
#include "GrayBMP.h"
#include "Util.cpp"
#include "Filter.h"
#include "Vector.h"
#include "Matrix.h"
#include "Draw.h"
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
	draw::Circle(gaussian,200,200,40);
	draw::Line(gaussian,200,200,400,400);
	draw::Line(gaussian,200,200,200,400);
	draw::Line(gaussian,200,200,400,200);
	draw::Cross(gaussian,200,200,5);
	draw::Cross(gaussian,200,400,5);
	draw::Cross(gaussian,400,200,5);
	draw::Cross(gaussian,400,400,5);
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
	
	double Var=3;
	int Range=5;
	int mid=2;
	Vector<float> gaussian(Range*Range);
	int k=0;
	for (int i = 0; i < 5; ++i)
	{
		for(int j=0; j < 5; ++j)
		{
			gaussian[k++]=Math::Gaussian(i-mid,j-mid,Var);
			printf("%lf  ",gaussian[k-1] );
		}
		printf("\n");
	}
	printf("%f\n",gaussian.sum() );
	cin.get();
}

void testOpticalFlow()
{
	BMP Input1,Input2;
	GrayBMP gray1,gray2;
	Input1.ReadFromFile("img/table1.bmp");
	Input2.ReadFromFile("img/table2.bmp");
	gray1.SetSize(Input1.TellWidth(),Input1.TellHeight());
	gray2.SetSize(Input1.TellWidth(),Input1.TellHeight());
	ConvertToGray(Input1,gray1);
	ConvertToGray(Input2,gray2);

	
	gray1=DownSampling(gray1,2);
	gray2=DownSampling(gray2,2);
	GrayBMP dx(gray1.TellWidth(),gray1.TellHeight()),
	        dy(gray1.TellWidth(),gray1.TellHeight()),
	        dt(gray1.TellWidth(),gray1.TellHeight());
	Dx(gray1,dx);
	Dy(gray1,dy);
	dt=gray2-gray1;
	WriteToFile(dt,"Dt.bmp");

	int Range=3;
	int mid=1;
	float Var=1;
	Vector<float> gaussian(Range*Range);
	int k=0;
	for (int i = 0; i < Range; ++i)
	{
		for(int j=0; j < Range; ++j)
		{
			gaussian[k++]=Math::Gaussian(i-mid,j-mid,Var);
		}
	}

	GrayBMP of(gray1.TellWidth(),gray1.TellHeight());
	for (int i = 0; i < gray1.TellWidth(); i+=10)
	{
		for (int j = 0; j < gray1.TellHeight(); j+=10)
		{
			if (Limit::OutOfRange(i,Range,0,gray1.TellWidth()-1) ||
				Limit::OutOfRange(j,Range,0,gray1.TellHeight()-1))
			{
				gray2(i,j)=255;
			}
			else
			{
				Vector<float> Ix,Iy,It;
				Ix=dx.GetSquare(i,j,Range);
				Iy=dy.GetSquare(i,j,Range);
				It=dt.GetSquare(i,j,Range);
				Ix=Ix.dotProduct(gaussian);
				Iy=Iy.dotProduct(gaussian);
				Matrix<float> mat(2,2);
				mat(0,0)=Ix*Ix;
				mat(0,1)=Ix*Iy;
				mat(1,0)=Ix*Iy;
				mat(1,1)=Iy*Iy;

				Vector<float> b(2),r(2);
				b[0]=Ix*It*(-1);
				b[1]=Iy*It*(-1);

				r=mat.inv()*b;
				draw::LineOffset(of,i,j,(int)r[0],(int)r[1]);
				// draw::Cross(of,i,j,5);
			//#define Debug
			#ifdef Debug
			Ix.printVec();
			Iy.printVec();
			It.printVec();
			mat.printMatrix();
			mat.inv().printMatrix();
			b.printVec();
			r.printVec();
			cout<<gray2(i,j)<<endl;
			cin.get();
			#endif
			}
		}
	}	
	WriteToFile(of,"of.bmp");
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
	// testGaussian();
	// testMatrix();
	testOpticalFlow();
	// testVector();
	// 
	// testDiff();
	// testOpticalFlow();

	return 0;
}