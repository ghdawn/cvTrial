#include "EasyBMP.h"
#include "GrayBMP.h"

void ConvertToGray(BMP &src,GrayBMP &dst);
void Dx(GrayBMP &src,GrayBMP &dx);
void Dy(GrayBMP &src,GrayBMP &dx);
void Sobel(GrayBMP &src,GrayBMP &dx);
void WriteToFile(GrayBMP &src,char* filename);