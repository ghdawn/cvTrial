#include "ImageProcess.h"
#include "Util.h"
#include "EasyBMP.h"
#include "GrayBMP.h"

void ConvertToGray(BMP &src, GrayBMP &dst)
{
	// convert each pixel to grayscale using RGB->YUV
	for (int i = 0; i < src.TellWidth(); i++)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			int Temp = (int) floor(
					0.299 * src(i, j)->Red + 0.587 * src(i, j)->Green
							+ 0.114 * src(i, j)->Blue);
			dst(i, j) = (Byte) Temp;
		}
	}
}

void WriteToFile(const GrayBMP &src, const char* filename)
{
	BMP temp;
	temp.SetSize(src.TellWidth(), src.TellHeight());
	temp.SetBitDepth(8);

	for (int i = 0; i < src.TellWidth(); i++)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{

			temp(i, j)->Red = src(i, j);
			temp(i, j)->Green = src(i, j);
			temp(i, j)->Blue = src(i, j);
		}
	}
	CreateGrayscaleColorTable(temp);
	temp.WriteToFile(filename);
}

void Dx(const GrayBMP &src, GrayBMP &dx)
{
	if (!dx.MatchSize(src.TellWidth(), src.TellHeight()))
	{
		dx.SetSize(src.TellWidth(), src.TellHeight());
	}
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if ((i == 0) || (j == 0) || (i == (src.TellWidth() - 1))
					|| (j == (src.TellHeight() - 1)))
			{
				dx(i, j) = src(i, j);
			}
			else
			{
				int TempBYTE = (src(i + 1, j - 1) - src(i - 1, j - 1))
						+ 2 * (src(i + 1, j) - src(i - 1, j))
						+ (src(i + 1, j + 1) - src(i - 1, j + 1));
				// int TempBYTE = Limit::Round((src(i + 1, j) - src(i - 1, j)) / 2.0);
				// int TempBYTE=(src(i+1,j-1)+2*src(i+1,j)+src(i+1,j+1)+(-1)*src(i-1,j-1)+(-2)*src(i-1,j)+(-1)*src(i-1,j+1));
				dx(i, j) = Limit::GrayByte(TempBYTE/8);
			}
		}
	}
}

void Dy(const GrayBMP &src, GrayBMP &dy)
{
	if (!dy.MatchSize(src.TellWidth(), src.TellHeight()))
	{
		dy.SetSize(src.TellWidth(), src.TellHeight());
	}
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if ((i == 0) || (j == 0) || (i == (src.TellWidth() - 1))
					|| (j == (src.TellHeight() - 1)))
			{
				dy(i, j) = src(i, j);
			}
			else
			{
				int TempBYTE = (src(i - 1, j - 1) - src(i - 1, j + 1))
						+ 2 * (src(i, j - 1) - src(i, j + 1))
						+ (src(i + 1, j - 1) - src(i + 1, j + 1));
				//int TempBYTE = Limit::Round((src(i, j + 1) - src(i, j - 1)) / 2.0);
				dy(i, j) = Limit::GrayByte(TempBYTE/8);
			}
		}
	}
}

void Sobel(const GrayBMP &src, GrayBMP &result)
{
	if (result.MatchSize(src.TellWidth(), src.TellHeight()))
	{
		result.SetSize(src.TellWidth(), src.TellHeight());
	}
	for (int i = 0; i < src.TellWidth(); ++i)
	{
		for (int j = 0; j < src.TellHeight(); ++j)
		{
			if ((i == 0) || (j == 0) || (i == (src.TellWidth() - 1))
					|| (j == (src.TellHeight() - 1)))
			{
				result(i, j) = src(i, j);
			}
			else
			{
				int dx = (src(i + 1, j - 1) - src(i - 1, j - 1))
						+ 2 * (src(i + 1, j) - src(i - 1, j))
						+ (src(i + 1, j + 1) - src(i - 1, j + 1))/8;
				int dy = (src(i - 1, j - 1) - src(i - 1, j + 1))
						+ 2 * (src(i, j - 1) - src(i, j + 1))
						+ (src(i + 1, j - 1) - src(i + 1, j + 1))/8;
				result(i, j) = Limit::GrayByte(Math::Abs(dx) + Math::Abs(dy));
			}
		}
	}
}

void DownSampling(const GrayBMP & bmp, GrayBMP& result, int scale)
{
	if (result.MatchSize(bmp.TellWidth() / scale, bmp.TellHeight() / scale))
	{
		result.SetSize(bmp.TellWidth() / scale, bmp.TellHeight() / scale);
	}
	for (int i = 0; i < result.TellWidth(); ++i)
	{
		for (int j = 0; j < result.TellHeight(); ++j)
		{
			result(i, j) = bmp(i * scale, j * scale);
		}
	}
}

void Scaling(const GrayBMP& src, GrayBMP& dst)
{
	const int FACTOR = 2048;
	const int BITS = 22;
	int width = dst.TellWidth(), height = dst.TellHeight();
	float fw = float(src.TellWidth()) / dst.TellWidth();
	float fh = float(src.TellHeight()) / dst.TellHeight();
	float x, y;
	int x0, y0, x1, y1;
	int u, v, u_1, v_1;
	int result;
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			x = i * fw;
			y = j * fh;
			x0 = (int) x;
			y0 = (int) y;
			u = (x - x0) * FACTOR;
			v = (y - y0) * FACTOR;
			u_1 = FACTOR - u;
			v_1 = FACTOR - v;
			result = (src(x0, y0) * u_1 + src(x0 + 1, y0) * u) * v_1
					+ (src(x0, y0 + 1) * u_1 + src(x0 + 1, y0 + 1) * u) * v;
			dst(i, j) = result >> BITS;
		}
}
