#ifndef _Gray_BMP_h_
#define _Gray_BMP_h_

#include "Vector.h"
typedef unsigned char Byte;
extern bool CanWarning;
void SetCanWarningOff( void );
void SetCanWarningOn( void );
bool GetCanWarningtate( void );

class GrayBMP
{
private:
	int Width;
	int Height;
	Byte** data;
public:
	void Dispose();
	int TellWidth() const;
	int TellHeight() const;
	Vector<float> GetSquare(int i,int j,int Range);
	Byte& operator()(int i,int j);
	Byte operator()(int i,int j) const;
	GrayBMP operator=(const GrayBMP& Input);
	void SetSize(int,int);
	GrayBMP();
	GrayBMP( GrayBMP& Input );
 	~GrayBMP();
};
#endif