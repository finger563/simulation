#include "pch.h"

#include "Matrix.h"

using namespace Base::Math;

// CONSTRUCTORS

template<int numRows, typename T>
Matrix<numRows, T>::Matrix()
{
	memset(vals, static_cast<T>(0), sizeof(T) * numVals);
}

template<int numRows, typename T>
Matrix<numRows, T>::Matrix(std::initializer_list<T> l)
{
	if (l.size() == numVals)
	{
		int n = 0;
		for (const T* it = begin(l); it != end(l); ++it)
		{
			vals[n++] = *it;
		}
	}
}

// MATH OPERATIONS
