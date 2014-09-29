#pragma once

#include <type_traits>
#include "Vector.h"

namespace Base
{
	namespace Math
	{
		template < int numRows, typename T >
		struct Matrix
		{
		public:
			T vals[numRows*numRows];
			int numVals = numRows * numRows;

			Matrix<numRows, T>();

			Matrix<numRows, T>(std::initializer_list<T> l);

			void ToIdentity();
			void ToRotationX(T angle);
			void ToRotationY(T angle);
			void ToRotationZ(T angle);
		};

		typedef Matrix< 2, float > Mat2x2f;
		typedef Matrix< 3, float > Mat3x3f;
		typedef Matrix< 4, float > Mat4x4f;

		typedef Matrix< 2, double > Mat2x2d;
		typedef Matrix< 3, double > Mat3x3d;
		typedef Matrix< 4, double > Mat4x4d;
	}
}

#include "Matrix.cpp" // needed to get the definitions of the functions