#ifndef _Gray_BMP_h_
#define _Gray_BMP_h_


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
	int TellWidth();
	int TellHeight();
	Byte& operator()(int i,int j);
	GrayBMP operator=(GrayBMP& Input);
	void SetSize(int,int);
	GrayBMP();
	GrayBMP( GrayBMP& Input );
 	~GrayBMP();
};
#endif