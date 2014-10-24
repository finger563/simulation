#include "pch.h"
#include "ROAM.h"

using namespace Renderer;

void roam::GenerateCube(std::vector<Base::Vertex>& verts, UINT depth)
{
	for (int i = 0; i < 12; i++)
		triangles.push_back(Renderer::Triangle());
#if 0
	0,1,3,		//0	
		2,3,1,	//1
		4,5,6,	//2
		7,6,5,	//3
		2,5,3,	//4
		4,3,5,	//5
		7,1,6,	//6
		0,6,1,	//7
		5,2,7,	//8
		1,7,2,	//9
		4,6,3,
		0,3,6
#endif
	// -z face
	triangles[0].left = &triangles[11];
	triangles[0].right = &triangles[7];
	triangles[0].base = &triangles[1];
	triangles[0].ia = 0;
	triangles[0].i0 = 3;
	triangles[0].i1= 1;
	triangles[1].left = &triangles[9];
	triangles[1].right = &triangles[4];
	triangles[1].base = &triangles[0];
	triangles[1].ia = 2;
	triangles[1].i0 = 1;
	triangles[1].i1 = 3;
	// +z face
	triangles[2].left = &triangles[10];
	triangles[2].right = &triangles[5];
	triangles[2].base = &triangles[3];
	triangles[2].ia = 4;
	triangles[2].i0 = 6;
	triangles[2].i1 = 5;
	triangles[3].left = &triangles[8];
	triangles[3].right = &triangles[6];
	triangles[3].base = &triangles[2];
	triangles[3].ia = 7;
	triangles[3].i0 = 5;
	triangles[3].i1 = 6;
	// +x face
	triangles[4].left = &triangles[8];
	triangles[4].right = &triangles[7];
	triangles[4].base = &triangles[5];
	triangles[4].ia = 2;
	triangles[4].i0 = 3;
	triangles[4].i1 = 5;
	triangles[5].left = &triangles[2];
	triangles[5].right = &triangles[10];
	triangles[5].base = &triangles[4];
	triangles[5].ia = 4;
	triangles[5].i0 = 5;
	triangles[5].i1 = 3;
	// -x face
	triangles[6].left = &triangles[3];
	triangles[6].right = &triangles[9];
	triangles[6].base = &triangles[7];
	triangles[6].ia = 7;
	triangles[6].i0 = 6;
	triangles[6].i1 = 1;
	triangles[7].left = &triangles[0];
	triangles[7].right = &triangles[11];
	triangles[7].base = &triangles[6];
	triangles[7].ia = 0;
	triangles[7].i0 = 1;
	triangles[7].i1 = 6;
	// +y face
	triangles[8].left = &triangles[3];
	triangles[8].right = &triangles[4];
	triangles[8].base = &triangles[9];
	triangles[8].ia = 5;
	triangles[8].i0 = 7;
	triangles[8].i1 = 2;
	triangles[9].left = &triangles[1];
	triangles[9].right = &triangles[6];
	triangles[9].base = &triangles[8];
	triangles[9].ia = 1;
	triangles[9].i0 = 2;
	triangles[9].i1 = 7;
	// -y face
	triangles[10].left = &triangles[5];
	triangles[10].right = &triangles[2];
	triangles[10].base = &triangles[11];
	triangles[10].ia = 4;
	triangles[10].i0 = 3;
	triangles[10].i1 = 6;
	triangles[11].left = &triangles[7];
	triangles[11].right = &triangles[0];
	triangles[11].base = &triangles[10];
	triangles[11].ia = 0;
	triangles[11].i0 = 6;
	triangles[11].i1 = 3;

	// add triangles to split queue
	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		split.insert(&triangles[i]);
	}
	// go through and split the queue
	std::set<Triangle*>::reverse_iterator tri = split.rbegin();
	while ((*tri)->level < depth)
	{
		recursiveSplit(verts, *tri, depth);
		tri = split.rbegin();
	}
	// determine error for each triangle in mesh
	// build split queue and determine diamonds (initialize split queue to base triangles)
	split.clear();
	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		split.insert(&triangles[i]);
	}
	// build merge queue
	merge.clear();
}

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
	std::set<Triangle*>::iterator tri = split.end();
	while ((*tri)->level < depth)
	{
		recursiveSplit(verts, *tri, depth);
		tri = split.end();
	}
	// determine error for each triangle in mesh
	// build split queue and determine diamonds (initialize split queue to base triangles)
	split.clear();
	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		split.insert(&triangles[i]);
	}
	// build merge queue
	merge.clear();
}

void roam::recursiveSplit(std::vector<Base::Vertex>& vBuffer, Triangle* tri, UINT depth)
{
	if (tri->level < depth)
	{
		Diamond *diamond = nullptr;
		bool newDiamond = true;
		UINT centerIndex = 0;
		if (tri->base->level == tri->level && tri->base->diamond != nullptr )	// base needs to be a parent of a diamond
		{
			newDiamond = false;		// diamond made by other split already parent[0], child[0,1] set up already in diamond
			diamond = tri->base->diamond;
			centerIndex = tri->base->diamond->children[0]->ia;
			diamond->parents[1] = tri;
		}
		if (newDiamond)
		{
			// make new vertex vc 
			Base::Vertex vc = Base::SphericalInterpolate(vBuffer[tri->i0], vBuffer[tri->i1], 0.5f, 2.0f, 0, 0, 0);
			// add vc into vertexBuffer
			vBuffer.push_back(vc);
			// update center index
			centerIndex = vBuffer.size() - 1;

			diamond = new Diamond();
			diamond->parents[0] = tri;
		}
		// add diamond pointer to tri
		tri->diamond = diamond;
		// split tri into t0 & t1
		Triangle t0, t1;
		t0.level = tri->level + 1;
		t0.error = tri->error * 0.5f;
		t1.level = tri->level + 1;
		t1.error = tri->error * 0.5f;
		t0.diamond = nullptr;
		t1.diamond = nullptr;	// triangles point to diamonds they are PARENTS of
		t0.base = tri->left;
		t1.base = tri->right;
		t0.ia = centerIndex;
		t1.ia = centerIndex;
		t0.i0 = tri->ia;
		t0.i1 = tri->i0;
		t1.i0 = tri->i1;
		t1.i1 = tri->ia;
		
		// add t0 and t1 to triangles list
		triangles.push_back(t0);
		triangles.push_back(t1);
		// update triangles left/right
		(--triangles.end() - 1)->left = &triangles[triangles.size() - 1];
		(--triangles.end() - 1)->right = nullptr;
		(--triangles.end())->left = nullptr;
		(--triangles.end())->right = &triangles[triangles.size() - 2];
		// update diamond with their pointer
		if (newDiamond)
		{
			diamond->parents[0] = tri;
			diamond->children[0] = &triangles[triangles.size() - 2];
			diamond->children[1] = &triangles[triangles.size() - 1];
		}
		else
		{
			diamond->parents[1] = tri;
			diamond->children[0]->right = &triangles[triangles.size() - 1];	// tb0 -> t1
			diamond->children[1]->left = &triangles[triangles.size() - 2];	// tb1 -> t0
			(--triangles.end() - 1)->right = diamond->children[1];			// t0 -> tb1
			(--triangles.end())->left = diamond->children[0];					// t1 -> tb0
			diamond->children[2] = &triangles[triangles.size() - 2];
			diamond->children[3] = &triangles[triangles.size() - 1];
			// diamond is complete, need to add it to merge list
			merge.insert(diamond);
		}

		// remove tri from split queue
		split.erase(tri);
		// add pointers to t0 and t1 to split queue
		UINT t0Ind = triangles.size() - 2;
		UINT t1Ind = triangles.size() - 1;
		split.insert(&triangles[t0Ind]);
		split.insert(&triangles[t1Ind]);
	}
}

void roam::nonGenerativeSplit(Triangle* tri)
{
	// ensure T and TB are same depth
	if (tri->base->level != tri->level)
	{
		nonGenerativeSplit(tri->base);
	}
	// remove the parents from the split q
	split.erase(tri->diamond->parents[0]);
	split.erase(tri->diamond->parents[1]);
	// add the children to the split q (all of them: t0,t1,tb0,tb1)
	split.insert(tri->diamond->children[0]);
	split.insert(tri->diamond->children[1]);
	split.insert(tri->diamond->children[2]);
	split.insert(tri->diamond->children[3]);
	// insert the new diamond (every split makes one)
	merge.insert(tri->diamond);
}

void roam::Split(float error)
{
	// take from front of split queue
	std::set<Triangle*>::reverse_iterator tri = split.rbegin();
	while ((*tri)->error > error)
	{
		nonGenerativeSplit(*tri);
		tri = split.rbegin();
	}
}

void roam::Merge(float error)
{
	// take from front of merge queue
	// while front of merge queue has less error than $error$
	//		merge front of merge queue : get parents from diamond
	//		take from front of merge queue
}

std::vector<UINT> roam::GetIndices()
{
	std::vector<UINT> retVec;
	for (std::set<Triangle*>::iterator it = split.begin(); it != split.end(); ++it)
	{
		retVec.push_back((*it)->i0);
		retVec.push_back((*it)->ia);
		retVec.push_back((*it)->i1);
	}
	return retVec;
}