typedef unsigned char Byte;
bool CanWarning=true;
class GrayBMP
{
private:
	int Width;
	int Height;
	Byte** data;
public:
	int TellWidth();
	int TellHeight();
	Byte& operator()(int i,int j);
	GrayBMP operator=(const GrayBMP& Input);
	
	GrayBMP();
	GrayBMP( GrayBMP& Input );
 	~GrayBMP();
}