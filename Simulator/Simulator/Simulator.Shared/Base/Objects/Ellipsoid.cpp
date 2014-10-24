#include "pch.h"
#include "Ellipsoid.h"

using namespace Base::Objects;

Ellipsoid::Ellipsoid()
{
	rootQT = nullptr;
}

Ellipsoid::Ellipsoid(float a, float b, float c)
{
	rootQT = nullptr;

	radius = VectorInit({ a, b, c });
	radiusSquared = VectorInit({ a*a, b*b, c*c });
}

Ellipsoid::~Ellipsoid()
{
	if (rootQT != nullptr)
		delete rootQT;
}
