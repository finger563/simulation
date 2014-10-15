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
		struct GameObject
		{
			Vector position;		// x,y,z notation
			Vector orientation;	// quaternion notation (q0,q1,q2,q3)
			Vector velocity;		// x,y,z velocities (relative to object orientation)

			// QuadTree for surface mesh (used by potentially physics and renderer subsystems)
			// Note that this is filled out separately
			DataStructures::QuadTreeNode*	rootQT;
			Mesh*								mesh;
			Vector scale;			// x,y,z scale factors

			BoundingSphere	boundingSphere;
			AABB			boundingBox;
			OOBB			orientedBoundingBox;

			GameObject() : rootQT(nullptr), mesh(nullptr), 
				position(Vector(DirectX::XMVectorSet(0,0,0,0))), velocity(position), orientation(Vector(velocity)),
				scale(Vector(DirectX::XMVectorSet(1, 1, 1, 0)))
			{}
			GameObject(Vector& pos) : GameObject() { position = pos; }
		};
	}
}