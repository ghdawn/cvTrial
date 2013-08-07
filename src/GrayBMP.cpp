	int GrayBMP::TellWidth()
	{
		return Width;
	}
	int GrayBMP:: GrayBMP::TellHeight()
	{
		return Height;
	}
	Byte& GrayBMP::operator()(int i,int j);
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
			printf() "Warning: Attempted to access non-existent pixel:(%d,%d)\n",i,j);
	 		return 0;
		}	
		return &(data[i][j]);
	}
GrayBMP GrayBMP::operator=(const GrayBMP& Input);

GrayBMP::GrayBMP();
GrayBMP::GrayBMP( GrayBMP& Input );
GrayBMP::~GrayBMP();