#ifndef _Util__
#define _Util__

#include "math.h"

typedef unsigned char Byte;

namespace Math
{
	const int PI=3.1415926;

	inline double Square(float a)	{ return a*a;}

	inline int Abs(int a){ return a>0 ? a : -a;}

	inline int Abs(float a){ return a>0 ? a : -a;}

	inline double Square(int a){ return a*a;}
	
	inline double Gaussian(int i,int j,double Var)
	{
		return exp(-(Square(i)+Square(j))/(2*Var*Var))/(2*PI*Var*Var);
	}


}

namespace Limit
{
	inline bool OutOfRange(int x,int Range, int Low, int High)
	{
		Range/=2;
		if (((x+Range)>High) || ((x-Range)<Low))
		{
			return true;
		}
		return false;
	}

	inline Byte GrayByte(int a)
	{ 
		if (a<0)
		{
			return (Byte)Math::Abs(a);
		}
		if(a>255)
		{
			return 255;
		}
	}
}
#endif