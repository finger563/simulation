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

template<typename T>
Vector<3, T>
Ellipsoid<T>::geodeticToNormal(Vector<3, T> geo)
{
	T cosLat = cos(geo[1]);
	Vector<3, T> normal({
		cosLat * cos(geo[0]),
		sin(geo.vals[1]),
		cosLat * sin(geo[0])
	});
	return normal.Normalized();
}

template<typename T>
Vector<3, T>
Ellipsoid<T>::normalToTangent(Vector<3, T> geo)
{
	// NEED TO GET ROTATION WORKED OUT
	Vector<3, T> tangent;
	return tangent.Normalized();
}

template<typename T>
Vector<3, T>
Ellipsoid<T>::normalToSurface(Vector<3, T> normal)
{
	Vector<3, T> k({
		radius2[0] * normal[0],
		radius2[1] * normal[1],
		radius2[2] * normal[2]
	});
	T gamma = sqrt(
		k[0] * n[0] +
		k[1] * n[1] +
		k[2] * n[2]
		);
	return Vector<3, T>({
		k[0] / gamma,
		k[1] / gamma,
		k[2] / gamma
	});
}

template<typename T>
Vector<2, T>
Ellipsoid<T>::geodeticToTexture(Vector<3, T> geo)
{
	return Vector<2, T>({
		geo[0] / ( 2.0 * PI),
		1.0 - (geo[1] / (PI) + 0.5)
	});
}