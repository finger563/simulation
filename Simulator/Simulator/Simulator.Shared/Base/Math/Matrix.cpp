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

template<int numRows, typename T>
Matrix<numRows, T>::ToIdentity()
{
	memset(vals, static_cast<T>(0), sizeof(T) * numVals);
	for (int i = 0; i < numRows; i++)
	{
		vals[i*i] = static_cast<T>(1.0);
	}
}

// MATH OPERATIONS

template<typename T>
Matrix<4, T> MatrixRotationX(T angle)
{
	Matrix<4, T> retMatrix;
	return retMatrix;
}

template<typename T>
Matrix<4, T> MatrixRotationY(T angle)
{
	Matrix<4, T> retMatrix;
	return retMatrix;
}

template<typename T>
Matrix<4, T> MatrixRotationX(T angle)
{
	Matrix<4, T> retMatrix;
	return retMatrix;
}