/*
 * Detector.cpp
 *
 *  Created on: 2013-8-29
 *      Author: ghdawn
 */

#include "Detector.h"
#include "Structure/Rect.h"
#include "Vector.h"
#include "GrayBMP.h"
#include "ImageProcess.h"

Detector::Detector(GrayBMP Patch)
{
    this->patch = Patch;
    rect.setSize(Patch.getWidth(), Patch.getHeight());
}

Rect Detector::MatchPatch(const GrayBMP& src)
{
    GrayBMP box;
    Vector<float> vbox, vpatch;
    vpatch = ImgProcess::toVector(patch);
    float best = 999999999, now;
    Rect result;
    result.setSize(rect.getWidth(), rect.getHeight());
    for (int j = 0; j < src.getHeight() - rect.getHeight(); ++j)
        for (int i = 0; i < src.getWidth() - rect.getWidth(); ++i)
        {
            rect.setPosition(i, j);
            ImgProcess::GetSquare(src, box, rect);
            vbox = ImgProcess::toVector(box) - vpatch;
            now = vbox.norm1();
            if (now < best)
            {
                best = now;
                result.setPosition(i, j);
            }
        }
    return result;
}
