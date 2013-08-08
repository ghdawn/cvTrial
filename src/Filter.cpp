#include "Filter.h"
#include "Util.cpp"
#include "Vector.h"

void Filter_Gaussian(GrayBMP &src,GrayBMP &dst, int Range, float Var)
{
	int mid=Range/2;
	Vector<float> gaussian(Range*Range);
	int k=0;
	for (int i = 0; i < Range; ++i)
	{
		for(int j=0; j < Range; ++j)
		{
			gaussian[k++]=Math::Gaussian(i-mid,j-mid,Var);
		}
	}
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if (Math::OutOfRange(i,Range,0,src.TellWidth()-1) ||
				Math::OutOfRange(j,Range,0,src.TellHeight()-1))
			{
				dst(i,j)=src(i,j);
			}
			else
			{
				Vector<float> origin;
				origin=src.GetSquare(i,j,Range);
				dst(i,j)=(Byte)(origin*gaussian);
			}
		}
	}	
}

void Filter_Median(GrayBMP &src,GrayBMP &dst, int Range)
{
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if (Math::OutOfRange(i,Range,0,src.TellWidth()-1) ||
				Math::OutOfRange(j,Range,0,src.TellHeight()-1))
			{
				dst(i,j)=src(i,j);
			}
			else
			{
				Vector<float> origin;
				origin=src.GetSquare(i,j,Range);
				origin.Sort();
				dst(i,j)=(Byte)(origin[Range*Range/2+1]);
			}
		}
	}		
}