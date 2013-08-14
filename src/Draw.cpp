#include "Draw.h"
#include "GrayBMP.h"

namespace draw
{
	void Circle(GrayBMP& bmp,int x,int y,int r)
	{
		int i,j;
		int width=bmp.TellWidth();
		int height=bmp.TellHeight();
		for(i=-r;i<r;i++)
		{
			j=sqrt(r*r-i*i);
			bmp((i+x)%width,(j+y)%height)=255;
			bmp((i+x)%width,(-j+y)%height)=255;
			bmp((j+x)%width,(i+y)%height)=255;
			bmp((-j+x)%width,(i+y)%height)=255;
		}
	}

	void LineOffset(GrayBMP &bmp,int x,int y,int offsetx,int offsety)
	{
		Line(bmp,x,y,x+offsetx,y+offsety);
	}

	void Line(GrayBMP &bmp,int x1,int y1,int x2,int y2)//绘制直线函数
	{
		double k;
		double b;
		int x,y;
		int width=bmp.TellWidth();
		int height=bmp.TellHeight();
		if (abs(x1-x2)<abs(y1-y2))
		{
			k=(x1-x2)*1.0/(y1-y2);
			b=x1-k*y1;
			if(y1>y2)
			{
				int a=y1;
				y1=y2;
				y2=a;
			}
			for(int i=y1;i<=y2;i++)
			{
				x=k*i+b;
				bmp((width+x)%width,(i+height)%height)=255;              
			}
		}
		else
		{
			k=(y1-y2)*1.0/(x1-x2);
			b=y1-k*x1;
			if(x1>x2)
			{
				int a=x1;
				x1=x2;
				x2=a;
			}
			for(int i=x1;i<=x2;i++)
			{
				y=k*i+b;
				bmp((width+i)%width,(y+height)%height)=255;              
			}
		}
	}

	void Cross(GrayBMP &bmp,int x,int y,int scale)//显示屏指针显示函数
	{
		LineOffset(bmp,x-scale,y,scale+scale,0);
		LineOffset(bmp,x,y-scale,0,scale+scale);
	}
}