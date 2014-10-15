#pragma once

#include "..\Objects\BoundingObjects.h"

namespace Base
{
	namespace DataStructures
	{
		struct QuadTreeNode
		{
			QuadTreeNode				*parent;		// parent of this node
			std::vector<QuadTreeNode*>	children;		// array of children points of this node
			float						error;			// deviation of this node from the actual mesh
			std::vector<uint32>			indices;		// array of indices for this node's mesh

			Objects::BoundingSphere	boundingSphere;
			Objects::AABB			boundingBox;
			Objects::OOBB			orientedBoundingBox;

			QuadTreeNode(
				QuadTreeNode	*_parent = nullptr,
				float			_error = 100.0f
				) : parent(_parent), error(_error) {}
		};
	}
}

#include "Quadtree.cpp"