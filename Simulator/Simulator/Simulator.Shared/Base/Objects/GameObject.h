#pragma once

#include "..\Math\Vector.h"
#include "..\Math\Matrix.h"
#include "BoundingObjects.h"

namespace Base
{
	namespace Objects
	{
		template<typename T>
		struct GameObject
		{
			Vector<3, T> position;		// x,y,z notation
			Vector<4, T> orientation;	// quaternion notation (q0,q1,q2,q3)
			Vector<3, T> velocity;		// x,y,z velocities (relative to object orientation)

			BoundingSphere<T>	boundingSphere;
			AABB<T>				boundingBox;
			OOBB<T>				orientedBoundingBox;
		};
	}
}