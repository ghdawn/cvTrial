#include "Vector.h"
#include "GrayBMP.h"
#include "algorithm"

template<typename T>
Vector<T>::Vector()
{

}

template<typename T>
Vector<T>::Vector(int Capability)
{
    data.resize(Capability);
}

template<typename T>
Vector<T>::~Vector()
{

}

template<typename T>
int Vector<T>::Length()
{
	return data.Size();
}


template<typename T>
void Vector<T>::Add(T t)
{
	data.push_back(t);
}


template<typename T>
void Vector<T>::Sort()
{
	std::sort(data.begin(),data.end());
}

template<typename T>
T Vector<T>::operator*(Vector vec)
{
	T ans=0;
	for (int i = 0; i < vec.Length(); ++i)
	{
		ans+=data[i]*vec[i];
	}
	return ans;
}

template<typename T>
Vector<T> Vector<T>::operator*(double k)
{
	for (int i = 0; i < Length(); ++i)
	{
		data[i]=T(data[i]*k);
	}
	return *this;
}

template<typename T>
Vector<T> Vector<T>::operator+(T t)
{
	for (int i = 0; i < Length(); ++i)
	{
		data[i]+=t;
	}
	return *this;
}

template<typename T>
T& Vector<T>::operator[](int i)
{
	return data[i];
}

template<typename T>
Vector<T> Vector<T>::operator=(Vector vec)
{
	data.resize(vec.Length());
	for (int i = 0; i < Length(); ++i)
	{
		data[i]=vec[i];
	}
	return *this;
}