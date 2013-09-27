/*
 * FeaturePoint.h
 *
 *  Created on: 2013-9-27
 *      Author: ghdawn
 */

#ifndef FEATUREPOINT_H_
#define FEATUREPOINT_H_

class FeaturePoint
{
    public:
        int x,y;
        int key;
        bool operator<(FeaturePoint b)
        {
            return key<b.key;
        }
};


#endif /* FEATUREPOINT_H_ */
