/*
 * OpticalFlow.h
 *
 *  Created on: 2013-9-11
 *      Author: ghdawn
 */

#ifndef OPTICALFLOW_H_
#define OPTICALFLOW_H_

class GrayBMP;
/*
 *
 */
class OpticalFlow
{
    public:
        void LKMethod(const GrayBMP& It1,const GrayBMP& It2,GrayBMP& u,GrayBMP& v);
    private:
        void GeneratePyramidal(const GrayBMP& It1,const GrayBMP& It2);
        int width[3];
        int height[3];
        GrayBMP I[3],J[3];
};

#endif /* OPTICALFLOW_H_ */
