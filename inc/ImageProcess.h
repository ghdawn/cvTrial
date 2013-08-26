#ifndef __IMAGE_PROCESS_H
#define __IMAGE_PROCESS_H

class BMP;
class GrayBMP;
void ConvertToGray(BMP &src,GrayBMP &dst);
void Dx(const GrayBMP &src,GrayBMP &dx);
void Dy(const GrayBMP &src,GrayBMP &dx);
void Sobel(const GrayBMP &src,GrayBMP &dx);
void WriteToFile(const GrayBMP &src,const char* filename);
void DownSampling(const GrayBMP & bmp, GrayBMP& result, int scale);
void Scaling(const GrayBMP& src,GrayBMP& dst);

#endif
