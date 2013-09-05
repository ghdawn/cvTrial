/*
 * Atemplate.h
 *
 *  Created on: 2013-8-28
 *      Author: ghdawn
 */

#ifndef ATEMPLATE_H_
#define ATEMPLATE_H_
#include<stdio.h>

template<typename T>
class ATClass
{
private:
    T* data;
    int _dim;


public:
    ATClass();
    ATClass(int dim,T* Data);
    ~ATClass();
    inline int dim() const { return _dim; }

};



template<typename T>
inline ATClass<T>::ATClass()
{
    _dim=0;
}

template<typename T>
inline ATClass<T>::ATClass(int dim, T* Data)
{
    _dim=dim;
}

#endif /* ATEMPLATE_H_ */
