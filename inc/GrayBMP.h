#ifndef _Gray_BMP_h_
#define _Gray_BMP_h_

class GrayBMP
{
    private:
        int width;
        int height;
        int* data;
    public:
        void Dispose();
        int getWidth() const;
        int getHeight() const;
        void SetSize(int, int);
        bool MatchSize(int, int);
        //Operator
        int& operator()(int i, int j);
        int operator()(int i, int j) const;
        GrayBMP& operator=(const GrayBMP& Input);
        GrayBMP operator-(const GrayBMP& Input);
        GrayBMP();
        GrayBMP(int Width, int Height);
        GrayBMP(GrayBMP& Input);
        ~GrayBMP();
};
#endif
