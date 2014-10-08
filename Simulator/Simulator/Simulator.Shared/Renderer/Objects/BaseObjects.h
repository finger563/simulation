#pragma once

#include "pch.h"
#include "Base\Base.h"

namespace Renderer
{
	static class BaseObjects
	{
	public:
		static void InitSphereMesh(Base::Mesh** meshPtr);
		static void InitCubeMesh(Base::Mesh** meshPtr);
	};
}
