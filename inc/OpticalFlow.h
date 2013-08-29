/*
 * OpticalFlow.h
 *
 *  Created on: 2013-8-29
 *      Author: ghdawn
 */

#ifndef OPTICALFLOW_H_
#define OPTICALFLOW_H_

class GrayBMP;

class OpticalFlow
{
    public:
        void LK_Method(GrayBMP gray1, const GrayBMP& gray2);
};

#endif /* OPTICALFLOW_H_ */
