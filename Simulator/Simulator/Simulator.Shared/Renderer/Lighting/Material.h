#pragma once

#include "pch.h"

using namespace DirectX;

namespace Renderer
{
	struct Material
	{
		XMVECTOR Diffuse;
		XMVECTOR Ambient;
		XMVECTOR Specular;
		XMVECTOR Reflect;
	};
}