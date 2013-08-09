#ifndef _Gray_BMP_h_
#define _Gray_BMP_h_

#include "Vector.h"

extern bool CanWarning;
void SetCanWarningOff( void );
void SetCanWarningOn( void );
bool GetCanWarningtate( void );

class GrayBMP
{
private:
	int Width;
	int Height;
	int** data;
public:
	void Dispose();
	int TellWidth() const;
	int TellHeight() const;
	Vector<float> GetSquare(int i,int j,int Range);
	void SetSize(int,int);

	//Operator
	int& operator()(int i,int j);
	int operator()(int i,int j) const;
	GrayBMP& operator=(const GrayBMP& Input);
	GrayBMP operator-(const GrayBMP& Input);
	GrayBMP();
	GrayBMP(int width,int height);
	GrayBMP( GrayBMP& Input );
 	~GrayBMP();
};
#endif