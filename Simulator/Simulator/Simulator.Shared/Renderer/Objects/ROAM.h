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

		Triangle() : diamond(nullptr), base(nullptr), left(nullptr), right(nullptr), error(1.0f), level(0) {}

		Triangle& operator = (const Triangle &t)
		{
			Triangle tmp(t);
			tmp.swap(*this);
			return *this;
		}

		void swap(Triangle &t)
		{
			std::swap(this->ia, t.ia);
			std::swap(this->i0, t.i0);
			std::swap(this->i1, t.i1);
			std::swap(this->level, t.level);
			std::swap(this->error, t.error);
			std::swap(this->base, t.base);
			std::swap(this->left, t.left);
			std::swap(this->right, t.right);
			std::swap(this->diamond, t.diamond);
		}

		inline bool operator < (const Triangle& rhs) const 
		{
			return this->error < rhs.error;
		}
		inline bool operator >(const Triangle& rhs) const
		{
			return !(*this < rhs);
		}
		inline bool operator == (const Triangle& rhs) const
		{
			return !(*this < rhs) && !(*this > rhs);
		}
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
		std::vector<Triangle> triangles;

		void recursiveSplit(std::vector<Base::Vertex>& vBuffer, Triangle* tri, UINT depth);
		void nonGenerativeSplit(Triangle* tri);
		void recursiveMerge();

	public:
		void GenerateCube(std::vector<Base::Vertex>& verts, UINT depth);
		// initialize the list of triangles and split them (setting up queues) until <depth> is reached
		void Init(std::vector<Base::Vertex>& verts, std::vector<Triangle>& initTris, UINT depth);
		// split all triangles in split queue whose error is greater than <error>
		void Split(float error);
		// merge all triangles in merge queue whose error is less than <error>
		void Merge(float error);
		// get the indices of all active triangles
		std::vector<UINT> GetIndices();
	};
}