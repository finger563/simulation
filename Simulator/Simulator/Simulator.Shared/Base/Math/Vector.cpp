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

// MATH OPERATIONS

template <int numElements, typename T>
T Vector<numElements,T>::dot(const Vector<numElements,T>& other)
{
	T retVal = 0;
	for (int i = 0; i < numElements; i++)
	{
		retVal += vals[i] * other.vals[i];
	}
	return retVal;
}