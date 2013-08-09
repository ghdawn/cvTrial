
#ifndef __Vector_H_
#define __Vector_H_

#include "vector"
#include "algorithm"
#include "string.h"
#include "iostream"
#include "cstdio"
#include "Util.cpp"
using namespace std;

template<typename T>
class Vector
{
private:
	T* data;
	int _dim;
	
	
public:
	Vector();
	Vector(int dim,T* Data=NULL);
	Vector(const Vector<T>& vec);
	~Vector();
	inline int dim() const { return _dim; }
	void Dispose(void);
	void allocate(int dim);
	void allocate(const Vector<T>& vec);
	bool matchDimension(const Vector<T>& vec) const;
	void loadData(int dim,T* data);
	void reset();
	inline void Sort(){std::sort(data,data+_dim);}
	void printVec();
	//Operator
	T operator*(const Vector<T> &vec) const;
	Vector<T> operator*(float k);
	Vector<T> operator+(T t);
	Vector<T> operator+(const Vector<T>&  t);
	inline T& operator[](int index){return data[index];}
	inline T operator[](int index) const {return data[index];};
	Vector<T>& operator=(const Vector& vec);

	//Calc
	Vector<T> dotProduct(const Vector<T> &vec) const;
	T norm1() const;
	float norm2() const;
	T sum() const;
};


template<typename T>
Vector<T>::Vector()
{
	data=NULL;
	_dim=0;
}
template<typename T>
void Vector<T>::Dispose()
{
	_dim=0;
	if (data==NULL)
	{
		return;
	}
	delete [] data;
}
template<typename T>
Vector<T>::Vector(int dim, T* Data)
{
    _dim=dim;
    data=new T[dim];
    if(Data!=NULL)
    {
    	for (int i = 0; i < _dim; ++i)
    	{
    		data[i]=Data[i];
    	}
    }
}
template<typename T>
Vector<T>::Vector(const Vector<T>& vec)
{
    _dim=vec.dim();
    data=new T[_dim];
    for (int i = 0; i < _dim; ++i)
    {
    	data[i]=vec[i];
    }
}
template<typename T>
Vector<T>::~Vector()
{
	Dispose();
}

template<typename T>
void Vector<T>::allocate(int dim)
{
	Dispose();
    _dim=dim;
    data=new T[dim];
    memset(data,0,_dim);
}
template<typename T>
void Vector<T>::allocate(const Vector<T>& vec)
{
	Dispose();
    _dim=vec.dim();
    data=new T[_dim];
    for (int i = 0; i < _dim; ++i)
    {
    	data[i]=vec[i];
    }
}


template<typename T>
bool Vector<T>::matchDimension(const Vector<T>& vec) const
{
	return dim==vec.dim();
}
template<typename T>
void Vector<T>::loadData(int dim,T* Data)
{
  	_dim=dim;
    data=new T[dim];
    if(Data!=NULL)
    {
    	for (int i = 0; i < _dim; ++i)
    	{
    		data[i]=Data[i];
    	}
    }
}
template<typename T>
void Vector<T>::reset()
{
	for (int i = 0; i < _dim; ++i)
    {
    	data[i]=0;
    }
}
template<typename T>
void Vector<T>::printVec()
{
	for (int i = 0; i < _dim; ++i)
	{
		cout<<data[i]<<' ';
	}
	cout<<endl;
}

template<typename T>
T Vector<T>::operator*(const Vector<T> &vec) const
{
	float ans=0;
	if (vec.dim()!=_dim)
	{
		printf("Vector's lenght should be equal!\n");
		return 0;
	}
	for (int i = 0; i < _dim; ++i)
	{
		// printf("(%d*%d=%d)\n", data[i],vec[i],data[i]*vec[i]);
		ans+=data[i]*vec[i];
	}
	return (T)ans;
}


template<typename T>
Vector<T> Vector<T>::operator*(float k)
{
	Vector<T> result(_dim);
	for (int i = 0; i < _dim; ++i)
	{
		result[i]=T(data[i]*k);
	}
	return result;
}

template<typename T>
Vector<T> Vector<T>::operator+(T t)
{
	Vector<T> result(_dim);
	for (int i = 0; i < dim(); ++i)
	{
		result[i]=data[i]+t;
	}
	return result;
}

template<typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& vec)
{
	Vector<T> result(vec.dim());
	if(matchDimension(vec))
	{
		for (int i = 0; i < dim(); ++i)
		{
			result[i]=data[i]+vec[i];
		}
	}
	else
	{
		printf("Dimension Error\n");
	}
	return result;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& vec)
{
	Dispose();
	_dim=vec.dim();
	data=new T[_dim];
	for (int i = 0; i < _dim; ++i)
	{
		data[i]=vec[i];
		//printf("%d:(%d->%d)\n", i,vec[i],this->data[i]);
	}
	return *this;
}

	//Calc
template<typename T>
Vector<T> Vector<T>::dotProduct(const Vector<T> &vec) const
{
	Vector<T> result(vec.dim());
	for (int i = 0; i < _dim; ++i)
	{
		result[i]=data[i]*vec[i];
	}
	return result;
}
template<typename T>
T Vector<T>::norm1() const
{
	T ans=0;
	for (int i = 0; i < _dim; ++i)
	{
		ans+=Math::Abs(data[i]);
	}
	return ans;
}
template<typename T>
float Vector<T>::norm2() const
{
	T ans=0;
	for (int i = 0; i < _dim; ++i)
	{
		ans+=data[i]*data[i];
	}
	return sqrt(ans);
}
template<typename T>
T Vector<T>::sum() const
{
	T ans=0;
	for (int i = 0; i < _dim; ++i)
	{
		ans+=data[i];
	}
	return ans;
}
#endif