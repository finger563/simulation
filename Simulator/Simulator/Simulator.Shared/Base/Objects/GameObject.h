#pragma once

#include "..\Math\Vector.h"
#include "..\Math\Matrix.h"
#include "BoundingObjects.h"
#include "..\Data Structures\QuadTree.h"
#include "..\Mesh\Mesh.h"

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
			Mesh*								mesh;
			Vector<3, T> scale;			// x,y,z scale factors

			BoundingSphere<T>	boundingSphere;
			AABB<T>				boundingBox;
			OOBB<T>				orientedBoundingBox;

			GameObject() : rootQT(nullptr), mesh(nullptr), 
				position(Vector<3, T>({ 0, 0, 0 })), velocity(position), orientation(Vector<4, T>(velocity, 0)),
				scale(Vector<3, T>({ 1, 1, 1 }))
			{}
			GameObject(Vector<3, T>& pos) : GameObject(), position(pos) {}
		};
	}
}