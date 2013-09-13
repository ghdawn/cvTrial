/*
 * BasicStruct.h
 *
 *  Created on: 2013-9-13
 *      Author: ghdawn
 */

#ifndef BASICSTRUCT_H_
#define BASICSTRUCT_H_

class Point
{
    public:
        int x, y;
        Point(int x, int y)
        {
            this->x = x;
            this->y = y;
        }
        Point operator+(const Point& a)
        {
            Point b;
            b.x = x + a.x;
            b.y = y + a.y;
            return b;
        }
        Point operator-(const Point& a)
        {
            Point b;
            b.x = x - a.x;
            b.y = y - a.y;
            return b;
        }
        Point operator=(const Point& a)
        {
            x = a.x;
            y = a.y;
            return *this;
        }
};

#endif /* BASICSTRUCT_H_ */
