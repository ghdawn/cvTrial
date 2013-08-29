/*
 * Detector.h
 *
 *  Created on: 2013-8-29
 *      Author: ghdawn
 */

#ifndef DETECTOR_H_
#define DETECTOR_H_
#include "GrayBMP.h"
#include "Structure/Rect.h"

class Rect;
class GrayBMP;

class Detector
{
    public:
        Detector(GrayBMP Patch);
        Rect MatchPatch(const GrayBMP& src);
    private:
        GrayBMP patch;
        Rect rect;
};

#endif /* DETECTOR_H_ */
