#include "Draw.h"
#include "GrayBMP.h"

namespace draw
{
void Circle(GrayBMP& bmp, int x, int y, int r)
{
	int i, j;
	int width = bmp.TellWidth();
	int height = bmp.TellHeight();
	for (i = -r; i < r; i++)
	{
		j = sqrt(r * r - i * i);
		bmp((i + x) % width, (j + y) % height) = 255;
		bmp((i + x) % width, (-j + y) % height) = 255;
		bmp((j + x) % width, (i + y) % height) = 255;
		bmp((-j + x) % width, (i + y) % height) = 255;
	}
}

void LineOffset(GrayBMP &bmp, int X, int Y, int offsetx, int offsety)
{
	float l = sqrt(offsetx * offsetx + offsety * offsety);
	int width = bmp.TellWidth() - 1;
	int height = bmp.TellHeight() - 1;
	float x = X, y = Y;
//	printf("X:(%d,%d)\nU:(%d,%d)\n",X,Y,offsetx,offsety);
	for (int i = 0; i < l; ++i)
	{
		x += (offsetx / l);
		y += (offsety / l);
//		printf("%f,%f  ",x,y);
		if (Limit::OutOfRange((int) x, 0, 0, width)
				|| Limit::OutOfRange((int) y, 0, 0, height))
		{
			continue;
		}
		bmp((int) x, (int) y) = 255;
	}
}

void Line(GrayBMP &bmp, int x1, int y1, int x2, int y2) //绘制直线函数
{
	LineOffset(bmp, x1, y1, x2 - x1, y2 - y1);
}

void Cross(GrayBMP &bmp, int x, int y, int scale) //显示屏指针显示函数
{
	LineOffset(bmp, x - scale, y, scale + scale, 0);
	LineOffset(bmp, x, y - scale, 0, scale + scale);
}
}
