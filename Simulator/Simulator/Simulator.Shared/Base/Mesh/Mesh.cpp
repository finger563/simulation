#include "pch.h"
#include "Mesh.h"

using namespace Base;

Vertex LinearInterpolate(Vertex& v0, Vertex& v1, float t)
{
	Vertex retVert(v0);
	return retVert;
}

Vertex SphericalInterpolate(Vertex& v0, Vertex& v1, float t,
	float r, float x, float y, float z)
{
	Vertex retVert(v0);
	return retVert;
}

Vertex EllipsoidInterpolate(Vertex& v0, Vertex& v1, float t,
	float a, float b, float c, float x, float y, float z)
{
	Vertex retVert(v0);
	return retVert;
}