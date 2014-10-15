#pragma once

#include "pch.h"
#include "Base\Base.h"

namespace Renderer
{
	class BaseObjects
	{
	private:
		static void SubdivideLongestEdge(Base::Mesh** meshPtr, UINT start);
		static void SubdivideTriangle(Base::Mesh** meshPtr, UINT start);
		static void SubdividePlanarQuad(Base::Mesh** meshPtr, UINT start);
		static void SubdivideEquilateralTriangle(Base::Mesh** meshPtr, UINT start);
		static void Midpoint(Base::Mesh** meshPtr, UINT start);
	public:
		static void InitSphereMesh(Base::Mesh** meshPtr);
		static void InitCubeMesh(Base::Mesh** meshPtr);
		static void InitEllipsoidMesh(Base::Mesh** meshPtr);
	};
}
