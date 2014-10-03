#pragma once

#include "..\Math\Vector.h"
#include "..\Math\Matrix.h"
#include "BoundingObjects.h"
#include "..\Data Structures\QuadTree.h"

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

			// QuadTree for surface mesh (used by potentially physics and renderer subsystems)
			// Note that this is filled out separately
			DataStructures::QuadTreeNode<T>*	rootQT;	

			BoundingSphere<T>	boundingSphere;
			AABB<T>				boundingBox;
			OOBB<T>				orientedBoundingBox;
		};
	}
}