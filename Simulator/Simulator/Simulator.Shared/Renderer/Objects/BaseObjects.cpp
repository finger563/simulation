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
		{ -1.0f, -1.0f, -1.0f, 0, 0, -1.0f },
		{ -1.0f, 1.0f, -1.0f, 0, 0, -1.0f },
		{ 1.0f, 1.0f, -1.0f, 0, 0, -1.0f },
		{ 1.0f, -1.0f, -1.0f, 0, 0, -1.0f },

		{ 1.0f, -1.0f, 1.0f, 0, 0, -1.0f },
		{ 1.0f, 1.0f, 1.0f, 0, 0, -1.0f },
		{ -1.0f, -1.0f, 1.0f, 0, 0, -1.0f },
		{ -1.0f, 1.0f, 1.0f, 0, 0, -1.0f }
	};
	(*meshPtr)->indices = 
	{
		0,1,3,
		2,3,1,
		4,5,6,
		7,6,5,
		2,5,3,
		4,3,5,
		7,1,6,
		0,6,1,
		5,2,7,
		1,7,2,
		4,6,3,
		0,3,6
	};
}

void BaseObjects::InitSphereMesh(Base::Mesh** meshPtr)
{
	InitCubeMesh(meshPtr);
	for (unsigned int i = 0; i < (*meshPtr)->vertices.size(); i++)
	{
		float magnitude =
			(*meshPtr)->vertices[i].pos[0] * (*meshPtr)->vertices[i].pos[0] +
			(*meshPtr)->vertices[i].pos[1] * (*meshPtr)->vertices[i].pos[1] +
			(*meshPtr)->vertices[i].pos[2] * (*meshPtr)->vertices[i].pos[2];
		magnitude = sqrt(magnitude);

		(*meshPtr)->vertices[i].pos[0] = (*meshPtr)->vertices[i].pos[0] / magnitude;
		(*meshPtr)->vertices[i].pos[1] = (*meshPtr)->vertices[i].pos[1] / magnitude;
		(*meshPtr)->vertices[i].pos[2] = (*meshPtr)->vertices[i].pos[2] / magnitude;

		(*meshPtr)->vertices[i].normal[0] = (*meshPtr)->vertices[i].pos[0];
		(*meshPtr)->vertices[i].normal[1] = (*meshPtr)->vertices[i].pos[1];
		(*meshPtr)->vertices[i].normal[2] = (*meshPtr)->vertices[i].pos[2];
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
	float x = v2.pos[0] - v1.pos[0];
	float y = v2.pos[1] - v1.pos[1];
	float z = v2.pos[2] - v1.pos[2];
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