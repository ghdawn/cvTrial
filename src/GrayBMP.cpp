#include "GrayBMP.h"
#include "stdio.h"
#include "Util.h"

int GrayBMP::TellWidth() const
{
	return Width;
}
int GrayBMP::TellHeight() const
{
	return Height;
}
Vector<float> GrayBMP::GetSquare(int x, int y, int Range)
{
	// printf("(%d,%d)\n", x,y);
	Vector<float> vec(Range * Range);
	int k = 0;
	Range /= 2;
	for (int i = x - Range; i <= x + Range; ++i)
	{
		for (int j = y - Range; j <= y + Range; ++j)
		{
			vec[k++] = data[i][j];
			// printf("%d ",data[i][j] );
		}
		// printf("\n");
	}
	return vec;
}

bool GrayBMP::MatchSize(int width, int height)
{
	return (width == Width) && (height == Height);
}

int& GrayBMP::operator()(int i, int j)
{
	bool Warn = false;
	if (i >= Width)
	{
		Warn = true;
	}
	if (i < 0)
	{
		Warn = true;
	}
	if (j >= Height)
	{
		Warn = true;
	}
	if (j < 0)
	{
		Warn = true;
	}
	if (Warn)
	{
		printf("Warning: Attempted to access non-existent pixel:(%d,%d)\n", i,
				j);
		return data[0][0];
	}
	return (data[i][j]);
}

int GrayBMP::operator()(int i, int j) const
{
	bool Warn = false;
	if (i >= Width)
	{
		Warn = true;
	}
	if (i < 0)
	{
		Warn = true;
	}
	if (j >= Height)
	{
		Warn = true;
	}
	if (j < 0)
	{
		Warn = true;
	}
	if (Warn)
	{
		printf("Warning: Attempted to access non-existent pixel:(%d,%d)\n", i,
				j);
		return data[0][0];
	}
	return (data[i][j]);
}
GrayBMP& GrayBMP::operator=(const GrayBMP& Input)
{
	SetSize(Input.TellWidth(), Input.TellHeight());
	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			data[i][j] = Input(i, j);
		}
	}
	return *this;
}

GrayBMP GrayBMP::operator-(const GrayBMP& Input)
{
	GrayBMP result(Width, Height);
	if (Width != Input.TellWidth() || Height != Input.TellHeight())
	{
		printf("width and height must  agree\n");
		return result;
	}
	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			result(i, j) = Limit::GrayByte(data[i][j] - Input(i, j));
		}
	}
	return result;
}
void GrayBMP::SetSize(int newWidth, int newHeight)
{
	Dispose();
	Width = newWidth;
	Height = newHeight;
	data = new int*[Width];

	for (int i = 0; i < Width; i++)
	{
		data[i] = new int[Height];
	}
}

GrayBMP::GrayBMP()
{
	Width = 1;
	Height = 1;
	data = new int*[Width];
	data[0] = new int[Height];
}

GrayBMP::GrayBMP(int width, int height)
{
	data = NULL;
	SetSize(width, height);
}
GrayBMP::GrayBMP(GrayBMP& Input)
{
	data = NULL;
	SetSize(Input.TellWidth(), Input.TellHeight());
	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			data[i][j] = Input(i, j);
		}
	}
}

void GrayBMP::Dispose()
{
	int i;
	if (data == NULL)
		return;
	for (i = 0; i < Width; i++)
	{
		delete[] data[i];
	}
	delete[] data;
}
GrayBMP::~GrayBMP()
{
	Dispose();
}
