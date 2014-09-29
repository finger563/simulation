#include "pch.h"

#include "Vector.h"

using namespace Base::Math;

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