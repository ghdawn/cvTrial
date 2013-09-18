#include "GrayBMP.h"
#include "stdio.h"
#include "Util.h"

int GrayBMP::getWidth() const
{
    return width;
}
int GrayBMP::getHeight() const
{
    return height;
}

bool GrayBMP::MatchSize(int width, int height)
{
    return (this->width == width) && (this->height == height);
}

int& GrayBMP::operator()(int i, int j)
{
    bool Warn = false;
    if (i >= width)
    {
        Warn = true;
        i = width - 1;
    }
    if (i < 0)
    {
        Warn = true;
        i = 0;
    }
    if (j >= height)
    {
        Warn = true;
        j = height - 1;
    }
    if (j < 0)
    {
        Warn = true;
        j = 0;
    }
    if (Warn)
    {
       // printf("Warning: Attempted to access non-existent pixel:(%d,%d)\n", i, j);
    }
    return (data[j * width + i]);
}

int GrayBMP::operator()(int i, int j) const
{
    bool Warn = false;
    if (i >= width)
    {
        Warn = true;
        i = width - 1;
    }
    if (i < 0)
    {
        Warn = true;
        i = 0;
    }
    if (j >= height)
    {
        Warn = true;
        j = height - 1;
    }
    if (j < 0)
    {
        Warn = true;
        j = 0;
    }
    if (Warn)
    {
        //printf("Warning: Attempted to access non-existent pixel:(%d,%d)\n", i,j);
        return 0;
    }
    return (data[j * width + i]);
}
GrayBMP& GrayBMP::operator=(const GrayBMP& Input)
{
    SetSize(Input.getWidth(), Input.getHeight());

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            data[j * width + i] = Input(i, j);
        }
    }

    return *this;
}

GrayBMP GrayBMP::operator-(const GrayBMP& Input)
{
    GrayBMP result(width, height);
    if (width != Input.getWidth() || height != Input.getHeight())
    {
        printf("width and height must  agree\n");
        return result;
    }

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            result(i, j) = Limit::GrayByte(data[j * width + i] - Input(i, j));
        }
    }
    return result;
}
void GrayBMP::SetSize(int Width, int Height)
{
    Dispose();
    width = Width;
    height = Height;
    data = new int[width * height]();

}

GrayBMP::GrayBMP()
{
    width = 1;
    height = 1;
    data = new int[1]();

}

GrayBMP::GrayBMP(int Width, int Height)
{
    data = NULL;
    SetSize(Width, Height);
}
GrayBMP::GrayBMP(GrayBMP& Input)
{
    data = NULL;
    SetSize(Input.getWidth(), Input.getHeight());

    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            data[j * width + i] = Input(i, j);
        }
    }
}

void GrayBMP::Dispose()
{
    int i;
    if (data == NULL)
        return;
    delete[] data;
    data = NULL;
}


GrayBMP::~GrayBMP()
{
    Dispose();
}
