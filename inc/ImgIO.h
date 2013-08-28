/*
 * ImgIO.h
 *
 *  Created on: 2013-8-28
 *      Author: ghdawn
 */

#ifndef IMGIO_H_
#define IMGIO_H_

class BMP;
class GrayBMP;

class ImgIO
{
    public:
           static void ReadFromFile(const char* filename,GrayBMP& src);
           static void ConvertToGray(BMP &src, GrayBMP &dst);
           static void WriteToFile(const GrayBMP &src, const char* filename);
};

#endif /* IMGIO_H_ */
