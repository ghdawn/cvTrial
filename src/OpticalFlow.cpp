/*
 * OpticalFlow.cpp
 *
 *  Created on: 2013-9-11
 *      Author: ghdawn
 */
#include "GrayBMP.h"
#include "OpticalFlow.h"

void OpticalFlow::LKMethod(const GrayBMP& It1, const GrayBMP& It2, GrayBMP& u,
        GrayBMP& v)
{
    GeneratePyramidal(It1, It2);
}

void OpticalFlow::GeneratePyramidal(const GrayBMP& It1, const GrayBMP& It2)
{
    width[0] = It1.getWidth();
    height[0] = It1.getHeight();
    I[0] = It1;
    J[0] = It2;
    for (int k = 1; k < 3; k++)
    {
        width[k] = width[k - 1] >> 1;
        height[k] = height[k - 1] >> 1;
        I[k].SetSize(width[k], height[k]);
        J[k].SetSize(width[k], height[k]);
        for (int i = 0; i < width[k]; ++i)
            for (int j = 0; j < height[k]; ++j)
            {
                I[k](i, j) = (I[k - 1]((i << 1), (j << 1))
                        >> 2
                                + (I[k - 1]((i << 1) - 1, (j << 1))
                                        + I[k - 1]((i << 1) + 1, (j << 1))
                                        + I[k - 1]((i << 1), (j << 1) - 1)
                                        + I[k - 1]((i << 1), (j << 1) + 1))
                        >> 3
                                + (I[k - 1]((i << 1) - 1, (j << 1) - 1)
                                        + I[k - 1]((i << 1) + 1, (j << 1) + 1)
                                        + I[k - 1]((i << 1) - 1, (j << 1) + 1)
                                        + I[k - 1]((i << 1) + 1, (j << 1) + 1))
                        >> 4);
                J[k](i, j) = (J[k - 1]((i << 1), (j << 1))
                        >> 2
                                + (J[k - 1]((i << 1) - 1, (j << 1))
                                        + J[k - 1]((i << 1) + 1, (j << 1))
                                        + J[k - 1]((i << 1), (j << 1) - 1)
                                        + J[k - 1]((i << 1), (j << 1) + 1))
                        >> 3
                                + (J[k - 1]((i << 1) - 1, (j << 1) - 1)
                                        + J[k - 1]((i << 1) + 1, (j << 1) + 1)
                                        + J[k - 1]((i << 1) - 1, (j << 1) + 1)
                                        + J[k - 1]((i << 1) + 1, (j << 1) + 1))
                        >> 4);
            }
    }

}

