#include "GrayBMP.h"
#include "stdio.h"
#include "string.h"

#ifndef _Can_Give_Warning_
#define _Can_Give_Warning_
bool CanWarning=true;
#endif

void SetCanWarningOff( void )
{ CanWarning = false; }
void SetCanWarningOn( void )
{ CanWarning = true; }
bool GetCanWarningtate( void )
{ return CanWarning; }

int GrayBMP::TellWidth() const
{
	return Width;
}
int GrayBMP::TellHeight() const
{
	return Height;
}
Vector<float> GrayBMP::GetSquare(int x,int y,int Range)
{
	// printf("(%d,%d)\n", x,y);
	Vector<float> vec(Range*Range);
	int k=0;
	Range/=2;
	for (int i = x-Range; i <= x+Range; ++i)
	{
		for(int j = y-Range; j<= y+Range; ++j)
		{
			vec[k++]=data[i][j];
			// printf("%d ",data[i][j] );
		}
		// printf("\n");
	}
	return vec;
}
Byte& GrayBMP::operator()(int i,int j)
{
	bool Warn = false;
	if( i >= Width )
		{  Warn = true; }
	if( i < 0 )
		{  Warn = true; }
	if( j >= Height )
		{  Warn = true; }
	if( j < 0 )
		{  Warn = true; }
	if( Warn && CanWarning )
	{
		printf( "Warning: Attempted to access non-existent pixel:(%d,%d)\n",i,j);
		return data[0][0];
	}	
	return (data[i][j]);
}

Byte GrayBMP::operator()(int i,int j) const
{
	bool Warn = false;
	if( i >= Width )
		{  Warn = true; }
	if( i < 0 )
		{  Warn = true; }
	if( j >= Height )
		{  Warn = true; }
	if( j < 0 )
		{  Warn = true; }
	if( Warn && CanWarning )
	{
		printf( "Warning: Attempted to access non-existent pixel:(%d,%d)\n",i,j);
		return data[0][0];
	}	
	return (data[i][j]);
}
GrayBMP GrayBMP::operator=(const GrayBMP& Input)
{
	Width=Input.TellWidth();
	Height=Input.TellHeight();
	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			data[i][j]=Input(i,j);
		}
	}
}

void GrayBMP::SetSize(int newWidth, int newHeight)
{
	Dispose();
	Width = newWidth;
	Height = newHeight;
	data = new Byte* [ Width ]; 

	for(int i=0; i<Width; i++)
		{ data[i] = new Byte [ Height ]; }
}

GrayBMP::GrayBMP()
{
	Width = 1;
	Height = 1;
	data = new Byte* [ Width ]; 
	data[0]=new Byte[Height];
}
GrayBMP::GrayBMP( GrayBMP& Input )
{
	data=NULL;
	SetSize(Input.TellWidth(),Input.TellHeight());
	for (int i = 0; i < Width; ++i)
	{
		for (int j = 0; j < Height; ++j)
		{
			data[i][j]=Input(i,j);
		}
	}
}

void GrayBMP::Dispose()
{
	int i;
	if(data==NULL) return;
	for(i=0;i<Width;i++)
		{ delete [] data[i]; }
	delete [] data;
}
GrayBMP::~GrayBMP()
{
	Dispose();
}