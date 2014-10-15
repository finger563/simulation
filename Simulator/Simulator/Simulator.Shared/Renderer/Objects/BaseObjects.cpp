#include "pch.h"
#include "BaseObjects.h"

using namespace Renderer;

void BaseObjects::InitCubeMesh(Base::Mesh** meshPtr)
{
	if (*meshPtr == nullptr)
		*meshPtr = new Base::Mesh();
	else
	{
		(*meshPtr)->indices.clear();
		(*meshPtr)->vertices.clear();
	}
	(*meshPtr)->vertices =
	{
		{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f },    // side 1
		{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },    // side 2
		{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f },

		{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f },    // side 3
		{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },

		{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },    // side 4
		{ 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
		{ -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f },

		{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f },    // side 5
		{ 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },

		{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },    // side 6
		{ -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
		{ -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
	};
	(*meshPtr)->indices = 
	{
		0, 1, 2,    // side 1
		2, 1, 3,
		4, 5, 6,    // side 2
		6, 5, 7,
		8, 9, 10,    // side 3
		10, 9, 11,
		12, 13, 14,    // side 4
		14, 13, 15,
		16, 17, 18,    // side 5
		18, 17, 19,
		20, 21, 22,    // side 6
		22, 21, 23,
	};
}

void BaseObjects::InitSphereMesh(Base::Mesh** meshPtr)
{
	InitCubeMesh(meshPtr);
	for (unsigned int i = 0; i < (*meshPtr)->vertices.size(); i++)
	{
		float magnitude =
			(*meshPtr)->vertices[i].x * (*meshPtr)->vertices[i].x +
			(*meshPtr)->vertices[i].y * (*meshPtr)->vertices[i].y +
			(*meshPtr)->vertices[i].z * (*meshPtr)->vertices[i].z;
		magnitude = sqrt(magnitude);

		(*meshPtr)->vertices[i].x = (*meshPtr)->vertices[i].x / magnitude;
		(*meshPtr)->vertices[i].y = (*meshPtr)->vertices[i].y / magnitude;
		(*meshPtr)->vertices[i].z = (*meshPtr)->vertices[i].z / magnitude;

		(*meshPtr)->vertices[i].nx = (*meshPtr)->vertices[i].x;
		(*meshPtr)->vertices[i].ny = (*meshPtr)->vertices[i].y;
		(*meshPtr)->vertices[i].nz = (*meshPtr)->vertices[i].z;	
	}
}

void BaseObjects::InitEllipsoidMesh(Base::Mesh** meshPtr)
{
	if (*meshPtr == nullptr)
		*meshPtr = new Base::Mesh();
	else
	{
		(*meshPtr)->indices.clear();
		(*meshPtr)->vertices.clear();
	}
	(*meshPtr)->vertices =
	{
	};
	(*meshPtr)->indices =
	{
	};
}

// Helper Functions

float VertexDistance(Base::Vertex& v1, Base::Vertex& v2)
{
	float x = v2.x - v1.x;
	float y = v2.y - v1.y;
	float z = v2.z - v1.z;
	return sqrt(x*x + y*y + z*z);
}

void BaseObjects::SubdivideLongestEdge(Base::Mesh** meshPtr, UINT start)
{

}

void BaseObjects::SubdivideTriangle(Base::Mesh** meshPtr, UINT start)
{
	SubdivideLongestEdge(meshPtr, start);	// find midpoint along longest edge
	// add new vertex to vertex vector
	// change index for original triangle
	// add new indices to index vector
}

void BaseObjects::SubdividePlanarQuad(Base::Mesh** meshPtr, UINT start)
{

}

void BaseObjects::SubdivideEquilateralTriangle(Base::Mesh** meshPtr, UINT start)
{

}

void BaseObjects::Midpoint(Base::Mesh** meshPtr, UINT start)
{

}