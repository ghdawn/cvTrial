
#ifndef __Vector_H_
#define __Vector_H_

#include "vector"
#include "algorithm"
#include "stdio.h"
using std::vector;

template<typename T>
class Vector
{
private:
	T* data;
	int _length;
	
	void Dispose();
public:
	Vector();
	Vector(int length);
	~Vector();
	inline int Length() const { return _length; }
	inline void Sort(){std::sort(data,data+_length);}
	T operator*(const Vector<T> &vec) const;
	Vector<T>& operator*(float k);
	Vector<T>& operator+(T t);
	inline T& operator[](int index){return data[index];}
	inline T operator[](int index) const {return data[index];};
	Vector<T>& operator=(const Vector& vec);
	/* data */
};


template<typename T>
Vector<T>::Vector()
{
	data=NULL;
	_length=0;
}
template<typename T>
void Vector<T>::Dispose()
{
	_length=0;
	if (data==NULL)
	{
		return;
	}
	delete [] data;
}
template<typename T>
Vector<T>::Vector(int length)
{
    _length=length;
    data=new T[length];
}

template<typename T>
Vector<T>::~Vector()
{
	Dispose();
}

template<typename T>
T Vector<T>::operator*(const Vector<T> &vec) const
{
	float ans=0;
	if (vec.Length()!=_length)
	{
		printf("Vector's lenght should be equal!\n");
		return 0;
	}
	for (int i = 0; i < _length; ++i)
	{
		// printf("(%d*%d=%d)\n", data[i],vec[i],data[i]*vec[i]);
		ans+=data[i]*vec[i];
	}
	return (T)ans;
}


template<typename T>
Vector<T>& Vector<T>::operator*(float k)
{
	for (int i = 0; i < _length; ++i)
	{
		data[i]=T(data[i]*k);
	}
	return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator+(T t)
{
	for (int i = 0; i < Length(); ++i)
	{
		data[i]+=t;
	}
	return *this;
}


template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& vec)
{
	Dispose();
	_length=vec.Length();
	data=new T[_length];
	for (int i = 0; i < _length; ++i)
	{
		data[i]=vec[i];
		//printf("%d:(%d->%d)\n", i,vec[i],this->data[i]);
	}
	return *this;
}
#endif