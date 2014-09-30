#pragma once

#include "..\Math\Vector.h"
#include "..\Math\Matrix.h"

namespace Base
{
	namespace Objects
	{
		template<typename T>
		struct Plane
		{
			Vector<3, T> point;
			Vector<3, T> normal;
		};

		template<typename T>
		struct BoundingSphere
		{
			Vector<3, T>	position;
			T				radius;
		};

		template<typename T>
		struct AABB	// axis-aligned bounding box
		{
			Vector<3, T> corner1, corner2;
		};

		template<typename T>
		struct OOBB	// object-oriented bounding box
		{
			Plane<T> planes[6];
		};
	}
}