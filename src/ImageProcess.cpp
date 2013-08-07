#include "ImageProcess.h"
#include "math.h"

void ConvertToGray(BMP &src,BMP &dst)
{
	// convert each pixel to grayscale using RGB->YUV
	for( int j=0 ; j < src.TellHeight() ; j++)
	{
		for( int i=0 ; i < src.TellWidth() ; i++)
		{
			int Temp = (int) floor( 0.299*src(i,j)->Red +
				0.587*src(i,j)->Green +
				0.114*src(i,j)->Blue );
			ebmpBYTE TempBYTE = (ebmpBYTE) Temp;
			dst(i,j)->Red = TempBYTE;
			dst(i,j)->Green = TempBYTE;
			dst(i,j)->Blue = TempBYTE;
		}
	}
	if( dst.TellBitDepth() < 16 )
		{ CreateGrayscaleColorTable( dst ); }
}

void Dx(BMP &src,BMP &dx)
{
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if ((i==0) || (j==0) || (i==(src.TellWidth()-1))||(j==(src.TellHeight()-1)))
			{
				dx.SetPixel(i,j,src.GetPixel(i,j));
			}
			else
			{
				ebmpBYTE TempBYTE=(src(i+1,j-1)->Red-src(i-1,j-1)->Red)+2*(src(i+1,j)->Red-src(i-1,j)->Red)+(src(i+1,j+1)->Red-src(i-1,j+1)->Red);
				dx(i,j)->Red = TempBYTE;
				dx(i,j)->Green = TempBYTE;
				dx(i,j)->Blue = TempBYTE;
			}
		}
	}
}


void Dy(BMP &src,BMP &dy)
{
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if ((i==0) || (j==0) || (i==(src.TellWidth()-1))||(j==(src.TellHeight()-1)))
			{
				dy.SetPixel(i,j,src.GetPixel(i,j));
			}
			else
			{
				ebmpBYTE TempBYTE=(src(i-1,j+1)->Red-src(i-1,j-1)->Red)+2*(src(i,j+1)->Red-src(i,j-1)->Red)+(src(i+1,j+1)->Red-src(i+1,j-1)->Red);
				dy(i,j)->Red = TempBYTE;
				dy(i,j)->Green = TempBYTE;
				dy(i,j)->Blue = TempBYTE;
			}
		}
	}
}

void Sobel(BMP &src,BMP &result)
{
	BMP dx,dy;
	dx.SetSize(src.TellWidth(),src.TellHeight());
	dx.SetBitDepth(8);
	dy.SetSize(src.TellWidth(),src.TellHeight());
	dy.SetBitDepth(8);
	CreateGrayscaleColorTable( dx );
	CreateGrayscaleColorTable( dy );
	Dx(src,dx);
	Dy(src,dy);
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			ebmpBYTE TempBYTE=sqrt(Square(dx(i,j)->Red)+Square(dy(i,j)->Red));
			result(i,j)->Red = TempBYTE;
			result(i,j)->Green = TempBYTE;
			result(i,j)->Blue = TempBYTE;
		}
	}
}