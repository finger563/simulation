#include "pch.h"
#include "Ellipsoid.h"

using namespace Base::Objects;

Ellipsoid::Ellipsoid()
{
	rootQT = nullptr;
}

Ellipsoid::Ellipsoid(double a, double b, double c)
{
	rootQT = nullptr;

	radius = DirectX::XMVectorSet(a, b, c, 0);
	radiusSquared = DirectX::XMVectorSet(a*a, b*b, c*c, 0);
}

Ellipsoid::~Ellipsoid()
{
	if (rootQT != nullptr)
		delete rootQT;
}

Vector
Ellipsoid::geodeticToNormal(Vector& geo)
{
	double cosLat = cos(DirectX::XMVectorGetByIndex(geo, 1));
	Vector normal(DirectX::XMVectorSet(
		cosLat * cos(DirectX::XMVectorGetByIndex(geo, 0)),
		sin(DirectX::XMVectorGetByIndex(geo, 1)),
		cosLat * sin(DirectX::XMVectorGetByIndex(geo, 0)),
		0
		));
	return DirectX::XMVector3Normalize(normal);
}

Vector
Ellipsoid::normalToTangent(Vector& normal)
{
	Vector tangent(DirectX::XMVectorSet(
		DirectX::XMVectorGetByIndex(normal, 0),
		0,
		DirectX::XMVectorGetByIndex(normal, 2),
		0
		));
	// NEED TO GET ROTATION WORKED OUT: ROTATE THIS BY PI/2 Y-AXIS
	return DirectX::XMVector3Normalize(tangent);
}

Vector
Ellipsoid::normalToSurface(Vector& normal, Vector& radius2)
{
	Vector k = DirectX::XMVector3Dot(radius2, normal);
	float gamma = DirectX::XMVectorGetByIndex(DirectX::XMVector3Length(DirectX::XMVector3Dot(k, normal)), 0);
	//k = k / gamma;
	return k;
}

Vector
Ellipsoid::geodeticToTexture(Vector& geo)
{
	return Vector(
		DirectX::XMVectorSet(
		DirectX::XMVectorGetByIndex(geo, 0) / (2.0 * M_PI),
		1.0 - (DirectX::XMVectorGetByIndex(geo, 1) / (M_PI)+0.5),
		0, 0
		)
		);
}