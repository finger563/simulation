#include "pch.h"
#include "ROAM.h"

using namespace Renderer;

void roam::Init(std::vector<Base::Vertex>& verts, std::vector<Triangle>& initTris, UINT depth)
{
	// copy initial triangles
	triangles.insert(triangles.end(), initTris.begin(), initTris.end());
	// add triangles to split queue
	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		split.insert(&triangles[i]);
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
			recursiveSplit(vBuffer, tri->base, tri->level);
		}
		// make new vertex vc 
		Base::Vertex vc = Base::SphericalInterpolate(vBuffer[tri->i0], vBuffer[tri->i1], 0.5f, 2.0f, 0, 0, 0);

		// add vc into vertexBuffer
		vBuffer.push_back(vc);

		// split tri into t0 & t1
		Triangle t0, t1;
		t0.level = tri->level + 1;
		t0.error = tri->error * 0.5f;
		t1.level = tri->level + 1;
		t1.error = tri->error * 0.5f;
		t0.base = tri->left;
		t1.base = tri->right;
		t0.ia = vBuffer.size() - 1;
		t1.ia = vBuffer.size() - 1;
		t0.i0 = tri->ia;
		t0.i1 = tri->i0;
		t1.i0 = tri->i1;
		t1.i1 = tri->ia;
		
		// split tri->base into tb0 & tb1
		Triangle tb0, tb1;
		tb0.level = tri->base->level + 1;
		tb0.error = tri->base->error * 0.5f;
		tb1.level = tri->base->level + 1;
		tb1.error = tri->base->error * 0.5f;
		tb0.base = tri->base->left;
		tb1.base = tri->base->right;
		tb0.ia = vBuffer.size() - 1;
		tb1.ia = vBuffer.size() - 1;
		tb0.i0 = tri->base->ia;
		tb0.i1 = tri->base->i0;
		tb1.i0 = tri->base->i1;
		tb1.i1 = tri->base->ia;

		// configure diamond pointer
		Diamond *diamond = new Diamond();
		diamond->parents[0] = tri;
		diamond->parents[1] = tri->base;

		t0.diamond = diamond;
		t1.diamond = diamond;
		tb0.diamond = diamond;
		tb1.diamond = diamond;

		// add new tris to triangle list
		triangles.push_back(t0);	// at size - 4
		triangles.push_back(t1);	// at size - 3
		triangles.push_back(tb0);	// at size - 2
		triangles.push_back(tb1);	// at size - 1

		(*(triangles.end()-3)).right = &triangles[triangles.size() - 1];
		(*(triangles.end() - 3)).left = &triangles[triangles.size() - 3];
		(*(triangles.end() - 2)).left = &triangles[triangles.size() - 2];
		(*(triangles.end() - 2)).right = &triangles[triangles.size() - 4];
		(*(triangles.end())).left = &triangles[triangles.size() - 4];
		(*(triangles.end())).right = &triangles[triangles.size() - 2];
		(*(triangles.end() - 1)).right = &triangles[triangles.size() - 3];
		(*(triangles.end() - 1)).left = &triangles[triangles.size() - 1];
		
		diamond->children[0] = &triangles[triangles.size() - 4];
		diamond->children[1] = &triangles[triangles.size() - 3];
		diamond->children[2] = &triangles[triangles.size() - 2];
		diamond->children[3] = &triangles[triangles.size() - 1];

		// remove tri from split queue
		split.erase(tri);
		// remove tri->base from split queue
		split.erase(tri->base);
		// add pointers to t0 and t1 to split queue
		split.insert(&triangles[triangles.size() - 4]);
		split.insert(&triangles[triangles.size() - 3]);
		// add pointers to tb0 and tb1 to split queue
		split.insert(&triangles[triangles.size() - 2]);
		split.insert(&triangles[triangles.size() - 1]);
		// add pointer to diamond to merge queue
		merge.insert(diamond);
		// call self on t0 and t1?
		// call self on tb0 and tb1?
	}
}

void roam::Split(std::vector<Base::Vertex>& vBuffer, float error)
{
	
}

void roam::Merge(std::vector<Base::Vertex>& vBuffer, float error)
{

}