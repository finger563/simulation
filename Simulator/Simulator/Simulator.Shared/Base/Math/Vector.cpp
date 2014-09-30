#include "pch.h"

#include "Vector.h"

using namespace Base::Math;

// CONSTRUCTORS

template <int numElements, typename T>
Vector<numElements, T>::Vector()
{
	memset(vals, static_cast<T>(0), sizeof(T) * numElements);
}

template <int numElements, typename T>
Vector<numElements, T>::Vector(Vector<numElements - 1, T>& other, T val)
{
	for (int i = 0; i < numElements - 1; i++)
	{
		vals[i] = other.vals[i];
	}
	vals[numElements - 1] = val;
}

template <int numElements, typename T>
Vector<numElements, T>::Vector(std::initializer_list<T> l)
{
	if (l.size() <= numElements)
	{
		int n = 0;
		for (const T* it = begin(l); it != end(l); ++it)
		{
			vals[n++] = *it;
		}
	}
}

template <int numElements, typename T>
T& Vector<numElements, T>::operator[](const int i)
{
	return vals[i];
}

// MATH OPERATIONS

template <int numElements, typename T>
T Vector<numElements,T>::Dot(const Vector<numElements,T>& other)
{
	T retVal = 0;
	for (int i = 0; i < numElements; i++)
	{
		retVal += vals[i] * other.vals[i];
	}
	return retVal;
}

template <int numElements, typename T>
T Vector<numElements, T>::Length()
{
	T square = 0;
	for (int i = 0; i < numElements; i++)
	{
		square += vals[i] * vals[i];
	}
	return sqrt(square);
}

template <int numElements, typename T>
Vector<numElements, T> Vector<numElements, T>::Normalized()
{
	Vector<numElements, T> retVec = *this;
	T len = retVec.Length();
	for (int i = 0; i < numElements; i++)
	{
		retVec.vals[i] = retVec.vals[i] / len;
	}
	return retVec;
}