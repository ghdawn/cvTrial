/*
 * ImgIO.cpp
 *
 *  Created on: 2013-8-28
 *      Author: ghdawn
 */

#include "ImgIO.h"
#include "EasyBMP.h"
#include "GrayBMP.h"

void ImgIO::ReadFromFile(const char* filename, GrayBMP& dst)
{
    BMP Input;
    Input.ReadFromFile(filename);
    dst.SetSize(Input.TellWidth(), Input.TellHeight());
    ConvertToGray(Input, dst);
}
void ImgIO::ConvertToGray(BMP &src, GrayBMP &dst)
{
    // convert each pixel to grayscale using RGB->YUV
    for (int j = 0; j < src.TellHeight(); ++j)
    {
        for (int i = 0; i < src.TellWidth(); ++i)
        {

            int Temp = (int) floor(
                    0.299 * src(i, j)->Red + 0.587 * src(i, j)->Green
                            + 0.114 * src(i, j)->Blue);
            dst(i, j) = Temp;
        }
    }
}

void ImgIO::WriteToFile(const GrayBMP &src, const char* filename)
{
    BMP temp;
    temp.SetSize(src.getWidth(), src.getHeight());
    temp.SetBitDepth(8);

    for (int j = 0; j < src.getHeight(); ++j)
    {
        for (int i = 0; i < src.getWidth(); i++)
        {

            temp(i, j)->Red = src(i, j);
            temp(i, j)->Green = src(i, j);
            temp(i, j)->Blue = src(i, j);
        }
    }
    CreateGrayscaleColorTable(temp);
    temp.WriteToFile(filename);
}
