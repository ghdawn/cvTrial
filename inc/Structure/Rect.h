/*
 * Rect.h
 *
 *  Created on: 2013-8-28
 *      Author: ghdawn
 */

#ifndef STRUCTURE_RECT_H_
#define STRUCTURE_RECT_H_

class Rect
{
    public:
        Rect()
        {
            x = y = 0;
            width = height = 0;
        }
        Rect(int x, int y, int range)
        {
            this->x = x;
            this->y = y;
            this->width = range;
            this->height = range;
        }
        Rect(int x, int y, int width, int height)
        {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
        }
        void setPosition(int x, int y);
        void setSize(int width, int height);
        int getHeight() const;
        int getWidth() const;
        int getX() const;
        int getY() const;
        Rect& operator=(const Rect& rect);
    private:
        int x, y;
        int width;
        int height;
};

inline int Rect::getHeight() const
{
    return height;
}

inline int Rect::getWidth() const
{
    return width;
}

inline int Rect::getX() const
{
    return x;
}

inline void Rect::setPosition(int x, int y)
{

    this->x = x;
    this->y = y;
}

inline void Rect::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
}

inline int Rect::getY() const
{
    return y;
}

inline Rect& Rect::operator =(const Rect& rect)
{
    this->x = rect.getX();
    this->y = rect.getY();
    this->width = rect.getWidth();
    this->height = rect.getHeight();
}

#endif /* STRUCTURE_RECT_H_ */
