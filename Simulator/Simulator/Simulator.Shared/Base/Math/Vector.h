#pragma once

namespace Base
{
	namespace Math
	{
		template< int numElements, typename T >
		struct Vector
		{
			T vals[numElements];
		
			Vector<numElements, T>();

			Vector<numElements, T>(Vector<numElements - 1, T>& other, T val);

			Vector<numElements, T>(std::initializer_list<T> l);

			T dot(const Vector<numElements, T>& other);
		};

		typedef Vector< 2, float > Vec2f;
		typedef Vector< 3, float > Vec3f;
		typedef Vector< 4, float > Vec4f;

		typedef Vector< 2, double > Vec2d;
		typedef Vector< 3, double > Vec3d;
		typedef Vector< 4, double > Vec4d;
	}
}

#include "Vector.cpp" // needed to get the definitions of the functions