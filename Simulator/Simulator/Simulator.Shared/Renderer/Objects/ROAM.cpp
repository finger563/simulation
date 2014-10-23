#include "pch.h"
#include "ROAM.h"

using namespace Renderer;

void roam::Init(std::vector<Base::Vertex>& initVerts, std::vector<Triangle>& initTris, UINT depth)
{
	// copy initial triangles 
	for (std::vector<Triangle>::iterator it = initTris.begin(); it != initTris.end(); ++it)
	{
		triangles.push_back(*it);	// add the initial triangles
	}
	// add triangles to split queue
	for (std::list<Triangle>::iterator it = triangles.begin(); it != triangles.end(); ++it)
	{
	}
	// go through and split the queue
	// determine error for each triangle in mesh
	// build split queue and determine diamonds (initialize split queue to base triangles)
	// build merge queue
}

void roam::recursiveSplit(std::vector<Base::Vertex>& vBuffer, Triangle& tri, UINT depth)
{
	if (tri.level < depth)
	{
		// split tri into t0 & t1
		// check diamond (base) level and induce more recursive splits there if necessary
		// configure diamond pointer
		// remove tri from split queue
		// add t0 and t1 to split queue
		// call self on t0 and t1
	}
}

void roam::Split(std::vector<Base::Vertex>& vBuffer, float error)
{
	
}

void roam::Merge(std::vector<Base::Vertex>& vBuffer, float error)
{

}