#include "ImageProcess.h"
#include "Util.cpp"


void ConvertToGray(BMP &src,GrayBMP &dst)
{
	// convert each pixel to grayscale using RGB->YUV
	for( int i=0 ; i < src.TellWidth() ; i++)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			int Temp = (int) floor( 0.299*src(i,j)->Red +
				0.587*src(i,j)->Green +
				0.114*src(i,j)->Blue );
			dst(i,j)=(Byte)Temp;
		}
	}
}

void WriteToFile(GrayBMP &src,const char* filename)
{
	BMP temp;
	temp.SetSize(src.TellWidth(),src.TellHeight());
	temp.SetBitDepth(8);
	
	for( int i=0 ; i < src.TellWidth() ; i++)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{

			temp(i,j)->Red=src(i,j);
			temp(i,j)->Green=src(i,j);
			temp(i,j)->Blue=src(i,j);
		}
	}
	CreateGrayscaleColorTable( temp );
	temp.WriteToFile(filename);
}

void Dx(GrayBMP &src,GrayBMP &dx)
{
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if ((i==0) || (j==0) || (i==(src.TellWidth()-1))||(j==(src.TellHeight()-1)))
			{
				dx(i,j)=src(i,j);
			}
			else
			{
				Byte TempBYTE=(src(i+1,j-1)-src(i-1,j-1))+2*(src(i+1,j)-src(i-1,j))+(src(i+1,j+1)-src(i-1,j+1));
				dx(i,j) = TempBYTE;
				
			}
		}
	}
}


void Dy(GrayBMP &src,GrayBMP &dy)
{
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if ((i==0) || (j==0) || (i==(src.TellWidth()-1))||(j==(src.TellHeight()-1)))
			{
				dy(i,j)=src(i,j);
			}
			else
			{
				Byte TempBYTE=(src(i-1,j-1)-src(i-1,j+1))+2*(src(i,j-1)-src(i,j+1))+(src(i+1,j-1)-src(i+1,j+1));
				dy(i,j) = TempBYTE;
				
			}
		}
	}
}

void Sobel(GrayBMP &src,GrayBMP &result)
{
	GrayBMP dx,dy;
	dx.SetSize(src.TellWidth(),src.TellHeight());
	dy.SetSize(src.TellWidth(),src.TellHeight());
	Dx(src,dx);
	Dy(src,dy);
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			Byte TempBYTE=(Byte)sqrt(Math::SquareInt(dx(i,j))+Math::SquareInt(dy(i,j)));
			result(i,j) = TempBYTE;
			
		}
	}
}