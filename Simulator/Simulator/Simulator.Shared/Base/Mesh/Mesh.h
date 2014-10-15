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
}