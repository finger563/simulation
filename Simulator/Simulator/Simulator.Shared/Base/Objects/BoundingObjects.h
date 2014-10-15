#pragma once

#include "..\Math\Vector.h"
#include "..\Math\Matrix.h"

namespace Base
{
	namespace Objects
	{
		struct Plane
		{
			Vector point;
			Vector normal;
		};

		struct BoundingSphere
		{
			Vector	position;
			double	radius;
		};

		struct AABB	// axis-aligned bounding box
		{
			Vector corner1, corner2;
		};

		struct OOBB	// object-oriented bounding box
		{
			Plane planes[6];
		};
	}
}