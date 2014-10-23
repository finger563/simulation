#pragma once

#include "pch.h"

namespace Base
{
	struct Vertex
	{
		float x, y, z;		// position vector
		float nx, ny, nz;	// normal vector
		float tx, ty, tz;	// tangent vector
		float u, v;			// texture coords [0..1]
	};
	struct Mesh
	{
		std::vector<Base::Vertex> vertices;
		std::vector<UINT> indices;
	};

	Vertex LinearInterpolate(Vertex& v0, Vertex& v1, float t);			// t is parameter : [0..1]
	Vertex SphericalInterpolate(Vertex& v0, Vertex& v1, float t,		// t is parameter : [0..1]
		float r, float x, float y, float z);							// radius, & position
	Vertex EllipsoidInterpolate(Vertex& v0, Vertex& v1, float t,		// t is parameter : [0..1]
		float a, float b, float c, float x, float y, float z);			// radius & position
}