#include "pch.h"
#include "Ellipsoid.h"

using namespace Base::Objects;

template<typename T>
Ellipsoid<T>::Ellipsoid()
{
	rootQT = nullptr;
	memset(radius.vals, static_cast<T>(0), sizeof(T) * 3);
	memset(radiusSquared.vals, static_cast<T>(0), sizeof(T) * 3);
}

template<typename T>
Ellipsoid<T>::Ellipsoid(T a, T b, T c)
{
	rootQT = nullptr;

	radius[0] = a;
	radius[1] = c;
	radius[2] = b;

	radiusSquared[0] = a*a;
	radiusSquared[1] = c*c;
	radiusSquared[2] = b*b;
}

template<typename T>
Ellipsoid<T>::~Ellipsoid()
{
	if (rootQT != nullptr)
		delete rootQT;
}
