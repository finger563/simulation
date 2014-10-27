#include "pch.h"
#include "ROAM.h"

using namespace Renderer;

void roam::GenerateCube(std::vector<Base::Vertex>* verts, UINT depth)
{
	vertices = verts;
	maxDepth = depth;
	int numtris = (1 << (maxDepth + 1)) - 1;
	std::vector<Triangle*> triangles;
	triangles.reserve(12);
	for (int i = 0; i < 12; i++)
	{
		Triangle* newTri = new Triangle();
		triangles.push_back(newTri);
	}
	// -z face
	triangles[0]->left = triangles[11];
	triangles[0]->right = triangles[7];
	triangles[0]->base = triangles[1];
	triangles[0]->ia = 0;
	triangles[0]->i0 = 3;
	triangles[0]->i1= 1;
	triangles[1]->left = triangles[9];
	triangles[1]->right = triangles[4];
	triangles[1]->base = triangles[0];
	triangles[1]->ia = 2;
	triangles[1]->i0 = 1;
	triangles[1]->i1 = 3;
	// +z face
	triangles[2]->left = triangles[10];
	triangles[2]->right = triangles[5];
	triangles[2]->base = triangles[3];
	triangles[2]->ia = 4;
	triangles[2]->i0 = 7;
	triangles[2]->i1 = 5;
	triangles[3]->left = triangles[8];
	triangles[3]->right = triangles[6];
	triangles[3]->base = triangles[2];
	triangles[3]->ia = 6;
	triangles[3]->i0 = 5;
	triangles[3]->i1 = 7;
	// +x face
	triangles[4]->left = triangles[1];
	triangles[4]->right = triangles[8];
	triangles[4]->base = triangles[5];
	triangles[4]->ia = 2;
	triangles[4]->i0 = 3;
	triangles[4]->i1 = 5;
	triangles[5]->left = triangles[2];
	triangles[5]->right = triangles[10];
	triangles[5]->base = triangles[4];
	triangles[5]->ia = 4;
	triangles[5]->i0 = 5;
	triangles[5]->i1 = 3;
	// -x face
	triangles[6]->left = triangles[3];
	triangles[6]->right = triangles[9];
	triangles[6]->base = triangles[7];
	triangles[6]->ia = 6;
	triangles[6]->i0 = 7;
	triangles[6]->i1 = 1;
	triangles[7]->left = triangles[0];
	triangles[7]->right = triangles[11];
	triangles[7]->base = triangles[6];
	triangles[7]->ia = 0;
	triangles[7]->i0 = 1;
	triangles[7]->i1 = 7;
	// +y face
	triangles[8]->left = triangles[3];
	triangles[8]->right = triangles[4];
	triangles[8]->base = triangles[9];
	triangles[8]->ia = 5;
	triangles[8]->i0 = 6;
	triangles[8]->i1 = 2;
	triangles[9]->left = triangles[1];
	triangles[9]->right = triangles[6];
	triangles[9]->base = triangles[8];
	triangles[9]->ia = 1;
	triangles[9]->i0 = 2;
	triangles[9]->i1 = 6;
	// -y face
	triangles[10]->left = triangles[5];
	triangles[10]->right = triangles[2];
	triangles[10]->base = triangles[11];
	triangles[10]->ia = 4;
	triangles[10]->i0 = 3;
	triangles[10]->i1 = 7;
	triangles[11]->left = triangles[7];
	triangles[11]->right = triangles[0];
	triangles[11]->base = triangles[10];
	triangles[11]->ia = 0;
	triangles[11]->i0 = 7;
	triangles[11]->i1 = 3;

	split.clear();
	// add triangles to split queue
	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		split.insert(triangles[i]);
	}
	// go through and split the queue
	Triangle* tri = *split.rbegin();
	while (tri->level < maxDepth)
	{
		generativeSplit(tri);
		tri = *split.rbegin();
	}
	// determine error for each triangle in mesh
	// build split queue and determine diamonds (initialize split queue to base triangles)
	split.clear();
	for (unsigned int i = 0; i < 12; i++)
	{
		split.insert(triangles[i]);
		triangles[i]->visible = true;
	}
	// build merge queue
	merge.clear();
}

void roam::generativeSplit(Triangle* tri)
{
	std::multiset<Triangle*>::iterator triIt = std::find(split.begin(), split.end(), tri);
	if (triIt != split.end())
	{
		split.erase(triIt);
	}
	bool generate = false;
	if (tri->t0 == nullptr) // haven't generated vertices & diamond
		generate = true;
	
	if (tri->base != nullptr && tri->base->base != tri) // need to make proper diamond
		generativeSplit(tri->base);	// need to make base point to tri through splitting
	
	if (generate)
	{
		tri->t0 = new Triangle();
		tri->t1 = new Triangle();

		tri->t0->level = tri->level + 1;
		tri->t0->error = tri->error * 0.5f;
		tri->t1->level = tri->level + 1;
		tri->t1->error = tri->error * 0.5f;

		tri->t0->i0 = tri->ia;
		tri->t0->i1 = tri->i0;

		tri->t1->i0 = tri->i1;
		tri->t1->i1 = tri->ia;
	}
	tri->t0->visible = true;
	tri->t1->visible = true;
	tri->visible = false;
	split.insert(tri->t0);
	split.insert(tri->t1);

	tri->t0->base = tri->left;
	tri->t0->left = tri->t1;
	tri->t1->base = tri->right;
	tri->t1->right = tri->t0;

	// update the left neighbor to point to t0
	if (tri->left->base == tri)
		tri->left->base = tri->t0;
	else if (tri->left->left == tri)
		tri->left->left = tri->t0;
	else if (tri->left->right == tri)
		tri->left->right = tri->t0;
	// update the right neighbor to point to t1
	if (tri->right->base == tri)
		tri->right->base = tri->t1;
	else if (tri->right->left == tri)
		tri->right->left = tri->t1;
	else if (tri->right->right == tri)
		tri->right->right = tri->t1;
	// update the base neighbor to point to t0 & t1
	if (tri->base->t0 != nullptr)
	{	// base has been split into children already
		if (generate)
		{
			tri->t0->diamond = tri->base->t0->diamond;
			tri->t1->diamond = tri->base->t0->diamond;	// triangles point to diamonds they are children of
			tri->t0->ia = tri->base->t0->ia;
			tri->t1->ia = tri->base->t0->ia;
			tri->t0->diamond->parents[1] = tri;
			tri->t0->diamond->children[2] = tri->t0;
			tri->t0->diamond->children[3] = tri->t1;
			tri->t0->diamond->error = tri->t0->error;
		}
		tri->base->t0->right = tri->t1;
		tri->base->t1->left = tri->t0;
		tri->t0->right = tri->base->t1;
		tri->t1->left = tri->base->t0;
	}
	else
	{	// base hasn't been split into children yet
		if (generate)		// make new vertex vc 
		{
			Base::Vertex vc = Base::SphericalInterpolate((*vertices)[tri->i0], (*vertices)[tri->i1], 0.5f, 1.0f, 0, 0, 0);
			//Base::Vertex vc = Base::LinearInterpolate((*vertices)[tri->i0], (*vertices)[tri->i1], 0.5f);
			// add vc into vertexBuffer
			vertices->push_back(vc);
			// update center index
			tri->t0->ia = vertices->size() - 1;
			tri->t1->ia = vertices->size() - 1;

			Diamond *diamond = new Diamond();
			tri->t0->diamond = diamond;
			tri->t1->diamond = diamond;
			diamond->parents[0] = tri;
			diamond->children[0] = tri->t0;
			diamond->children[1] = tri->t1;
		}
		generativeSplit(tri->base);
	}
}

void roam::nonGenerativeSplit(Triangle* tri)
{
	std::multiset<Triangle*>::iterator triIt = std::find(split.begin(), split.end(), tri);
	split.erase(triIt);

	// update the left neighbor to point to t0
	if (tri->left->base == tri)
		tri->left->base = tri->t0;
	else if (tri->left->left == tri)
		tri->left->left = tri->t0;
	else if (tri->left->right == tri)
		tri->left->right = tri->t0;
	// update the right neighbor to point to t1
	if (tri->right->base == tri)
		tri->right->base = tri->t1;
	else if (tri->right->left == tri)
		tri->right->left = tri->t1;
	else if (tri->right->right == tri)
		tri->right->right = tri->t1;

	tri->t0->base = tri->left;
	tri->t0->left = tri->t1;
	tri->t1->base = tri->right;
	tri->t1->right = tri->t0;
	tri->t0->visible = true;
	tri->t1->visible = true;
	tri->visible = false;
	split.insert(tri->t0);
	split.insert(tri->t1);

	if (tri->base->visible)	// base hasn't been split yet
		merge.insert(tri->t0->diamond);

	if (tri->base->base != tri) // need to make proper diamond
	{
		nonGenerativeSplit(tri->base);	// need to make base point to tri through splitting
	}
	// update the base neighbor to point to t0 & t1
	tri->base->t0->right = tri->t1;
	tri->base->t1->left = tri->t0;
	tri->t0->right = tri->base->t1;
	tri->t1->left = tri->base->t0;
}

void roam::recursiveMerge(Diamond* diamond)
{
	Triangle* tri;
	for (int i = 0; i < 4; i++)
	{
		tri = diamond->children[i];	
		tri->visible = false;
		std::multiset<Triangle*>::iterator triIt = std::find(split.begin(), split.end(), tri);
		if (triIt != split.end())
			split.erase(triIt);
	}
	split.insert(diamond->parents[0]);
	split.insert(diamond->parents[1]);

	//if (diamond->parents[0]->diamond != nullptr && diamond->parents[0]->diamond == diamond->parents[1]->diamond)
	//	merge.insert(diamond->parents[0]->diamond);

	tri = diamond->parents[0];
	tri->visible = true;
	tri->left = tri->t0->base;
	tri->right = tri->t1->base;
	tri->base = diamond->parents[1];
	// update the left neighbor to point to T
	if (tri->left->base == tri->t0)
		tri->left->base = tri;
	else if (tri->left->left == tri->t0)
		tri->left->left = tri;
	else if (tri->left->right == tri->t0)
		tri->left->right = tri;
	// update the right neighbor to point to T
	if (tri->right->base == tri->t1)
		tri->right->base = tri;
	else if (tri->right->left == tri->t1)
		tri->right->left = tri;
	else if (tri->right->right == tri->t1)
		tri->right->right = tri;

	tri = diamond->parents[1];
	tri->visible = true;
	tri->left = tri->t0->base;
	tri->right = tri->t1->base;
	tri->base = diamond->parents[0];
	// update the left neighbor to point to TB
	if (tri->left->base == tri->t0)
		tri->left->base = tri;
	else if (tri->left->left == tri->t0)
		tri->left->left = tri;
	else if (tri->left->right == tri->t0)
		tri->left->right = tri;
	// update the right neighbor to point to TB
	if (tri->right->base == tri->t1)
		tri->right->base = tri;
	else if (tri->right->left == tri->t1)
		tri->right->left = tri;
	else if (tri->right->right == tri->t1)
		tri->right->right = tri;
}

void roam::Split(float error)
{
	if (split.size() == 0)
		return;
	Triangle* tri = *split.rbegin();
	while (tri->error > error)
	{
		nonGenerativeSplit(tri);
		tri = *split.rbegin();
	}
}

void roam::Merge(float error)
{
	if (merge.size() == 0)
		return;
	Diamond* diamond = *merge.begin();
	while (diamond->error < error)	// want to get lowest error diamonds
	{
		recursiveMerge(diamond);
		merge.erase(merge.begin());
		if (merge.size() == 0)
			break;
		diamond = *merge.begin();
	}
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