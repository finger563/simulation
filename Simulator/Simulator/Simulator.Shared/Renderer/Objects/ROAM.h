#pragma once

#include "pch.h"
#include <set>

#include "Base\Mesh\Mesh.h"

namespace Renderer
{
	struct Diamond;
	struct Triangle
	{
		// va is opposite "base"
		// v0 is left edge
		// v1 is right edge
		UINT ia, i0, i1;		// indices into vertex buffer for this triangle
		UINT level;				// what level of the mesh is this triangle from?
		float error;			// what is the error between this triangle and its subtriangles
		Triangle* base;			// shares edge opposite va (ia)
		Triangle* left;			// shares edge to the left of midpoint (i0->ia)
		Triangle* right;		// shares edge to the right of midpoint (i1->ia)
		Diamond* diamond;		// set if the triangle is part of a diamond
	};

	struct Diamond
	{
		Triangle *parents[2];		// parent triangles (i.e. triangles that will remain after merge)
		Triangle *children[4];		// children (of the two parents)
	};

	class roam
	{
	private:
		std::set<Triangle*> split;
		std::set<Diamond*> merge;
		std::list<Triangle> triangles;

		void recursiveSplit(std::vector<Base::Vertex>& vBuffer, Triangle* tri, UINT depth);
		void recursiveMerge();

	public:
		// initialize the list of triangles and split them (setting up queues) until <depth> is reached
		void Init(std::vector<Base::Vertex>& initVerts, std::vector<Triangle>& initTris, UINT depth);
		// split all triangles in split queue whose error is greater than <error>
		void Split(std::vector<Base::Vertex>& vBuffer, float error);
		// merge all triangles in merge queue whose error is less than <error>
		void Merge(std::vector<Base::Vertex>& vBuffer, float error);
	};
}