#ifndef _INC_DRAW_H_
#define _INC_DRAW_H_

class GrayBMP;

namespace draw
{
	void Circle(GrayBMP &bmp, int x,int y,int r,int color);
	void Line(GrayBMP &bmp,int x1,int y1,int x2,int y2,int color);
	void LineOffset(GrayBMP &bmp,int x,int y,int offsetx,int offsety,int color);
	void Cross(GrayBMP &bmp,int x,int y,int scale,int color);
}
#endif
