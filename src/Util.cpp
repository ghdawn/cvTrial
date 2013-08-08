#ifndef _Util__
#define _Util__

#include "math.h"
namespace Math
{
	const int PI=3.1415926;

	inline double Square(double a)	{ return a*a;}

	inline double SquareInt(int a){ return a*a;}
	
	inline double Gaussian(int i,int j,double Var)
	{
		return exp(-(Square(i)+Square(j))/(2*Var*Var))/(2*PI*Var*Var);
	}

	inline bool OutOfRange(int x,int Range, int Low, int High)
	{
		Range/=2;
		if (((x+Range)>High) || ((x-Range)<Low))
		{
			return true;
		}
		return false;
	}
	
}
#endif