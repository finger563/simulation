#include "pch.h"
#include "ROAM.h"

using namespace Renderer;

void roam::Init(std::vector<Base::Vertex>& verts, std::vector<Triangle>& initTris, UINT depth)
{
	// copy initial triangles
	triangles.insert(triangles.end(), initTris.begin(), initTris.end());
	// add triangles to split queue
	for (std::vector<Triangle>::iterator it = triangles.begin(); it != triangles.end(); ++it)
	{
	}
	// go through and split the queue
	// determine error for each triangle in mesh
	// build split queue and determine diamonds (initialize split queue to base triangles)
	// build merge queue
}

void roam::recursiveSplit(std::vector<Base::Vertex>& vBuffer, Triangle* tri, UINT depth)
{
	if (tri->level < depth)
	{
		if (tri->level > tri->base->level)
		{
			recursiveSplit(vBuffer, tri->base, depth);
		}
		// split tri into t0 & t1
		Triangle t0, t1;
		// split tri->base into tb0 & tb1
		Triangle tb0, tb1;
		// configure diamond pointer
		Diamond diamond;
		diamond.parents[0] = tri;
		diamond.parents[1] = tri->base;
		// add new tris to triangle list
		// add new diamond to diamond list

		diamond.children[0] = &t0;
		diamond.children[1] = &t1;
		diamond.children[2] = &tb0;
		diamond.children[3] = &tb1;
		t0.diamond = &diamond;
		t1.diamond = &diamond;
		tb0.diamond = &diamond;
		tb1.diamond = &diamond;
		// remove tri from split queue
		// remove tri->base from split queue
		// add t0 and t1 to split queue
		// add tb0 and tb1 to split queue
		// add diamond to merge queue
		// call self on t0 and t1
	}
}

void roam::Split(std::vector<Base::Vertex>& vBuffer, float error)
{
	
}

void roam::Merge(std::vector<Base::Vertex>& vBuffer, float error)
{

}