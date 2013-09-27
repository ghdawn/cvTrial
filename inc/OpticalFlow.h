/*
 * OpticalFlow.h
 *
 *  Created on: 2013-9-11
 *      Author: ghdawn
 */

#ifndef OPTICALFLOW_H_
#define OPTICALFLOW_H_
#include <vector>
class GrayBMP;
class Point;
class Rect;
class FeaturePoint;

using std::vector;
/*
 *
 */
class OpticalFlow
{
    public:
        void Init(const GrayBMP& It1,const GrayBMP& It2);
        void Compute(Point& U,Point& V,bool Forward);
        Rect Compute(const Rect& rect);
        int level;
        OpticalFlow();
        OpticalFlow(int Level);
        void SelectGoodFeature(const Rect& rect,std::vector<FeaturePoint>& result);
    private:
        void GeneratePyramidal(const GrayBMP& It1,const GrayBMP& It2);
        float _minEigenvalue(float gxx, float gxy, float gyy);
        int Ixx(int L,int x,int y);
        int Ixy(int L,int x,int y);
        int Iyy(int L,int x,int y);
        int width[3];
        int height[3];
        GrayBMP I[3],J[3];
        GrayBMP dx[3],dy[3];
        GrayBMP ixx[3],ixy[3],iyy[3];
};

#endif /* OPTICALFLOW_H_ */
