#pragma once

#include "pch.h"

namespace Base
{
	struct Vertex
	{
		float x, y, z;
		float nx, ny, nz;
	};
	struct Mesh
	{
		std::vector<Base::Vertex> vertices;
		std::vector<UINT> indices;
	};
}