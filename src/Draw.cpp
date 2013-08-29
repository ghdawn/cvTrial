#include "Draw.h"
#include "GrayBMP.h"
#include "Util.h"
#include "Structure/Rect.h"

namespace draw
{
    void Circle(GrayBMP& bmp, int x, int y, int r, int color)
    {
        int i, j;
        int width = bmp.getWidth();
        int height = bmp.getHeight();
        for (i = -r; i < r; i++)
        {
            j = sqrt(r * r - i * i);
            bmp((i + x) % width, (j + y) % height) = color;
            bmp((i + x) % width, (-j + y) % height) = color;
            bmp((j + x) % width, (i + y) % height) = color;
            bmp((-j + x) % width, (i + y) % height) = color;
        }
    }

    void LineOffset(GrayBMP &bmp, int X, int Y, int offsetx, int offsety,
            int color)
    {
        float l = sqrt(offsetx * offsetx + offsety * offsety);
        int width = bmp.getWidth() - 1;
        int height = bmp.getHeight() - 1;
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
            bmp((int) x, (int) y) = color;
        }
    }

    void Line(GrayBMP &bmp, int x1, int y1, int x2, int y2, int color) //绘制直线函数
    {
        LineOffset(bmp, x1, y1, x2 - x1, y2 - y1, color);
    }

    void Cross(GrayBMP &bmp, int x, int y, int scale, int color) //显示屏指针显示函数
    {
        LineOffset(bmp, x - scale, y, scale + scale, 0, color);
        LineOffset(bmp, x, y - scale, 0, scale + scale, color);
    }

    void Rectangle(GrayBMP& bmp, Rect rect,int color)
    {
        int x=rect.getX(),y=rect.getY();
        int width=rect.getWidth(),height=rect.getHeight();
        LineOffset(bmp,x,y,width,0,color);
        LineOffset(bmp,x,y,0,height,color);
        LineOffset(bmp,x+width,y,0,height,color);
        LineOffset(bmp,x,y+height,width,0,color);
    }

}
