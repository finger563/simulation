#pragma once

#include "..\Objects\BoundingObjects.h"

namespace Base
{
	namespace DataStructures
	{
		template<typename T>
		struct QuadTreeNode
		{
			QuadTreeNode				*parent;		// parent of this node
			std::vector<QuadTreeNode*>	children;		// array of children points of this node
			float						error;			// deviation of this node from the actual mesh
			std::vector<uint32>			indices;		// array of indices for this node's mesh

			Objects::BoundingSphere<T>	boundingSphere;
			Objects::AABB<T>			boundingBox;
			Objects::OOBB<T>			orientedBoundingBox;

			QuadTreeNode(
				QuadTreeNode	*_parent = nullptr,
				float			_error = 100.0f
				);
		};
	}
}