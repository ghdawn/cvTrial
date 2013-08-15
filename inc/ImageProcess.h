#ifndef __IMAGE_PROCESS_H
#define __IMAGE_PROCESS_H

class BMP;
class GrayBMP;
void ConvertToGray(BMP &src,GrayBMP &dst);
void Dx(GrayBMP &src,GrayBMP &dx);
void Dy(GrayBMP &src,GrayBMP &dx);
void Sobel(GrayBMP &src,GrayBMP &dx);
void WriteToFile(GrayBMP &src,const char* filename);
GrayBMP DownSampling(const GrayBMP & bmp,int scale);

#endif
