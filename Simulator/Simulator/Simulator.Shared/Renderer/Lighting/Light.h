#pragma once

#include "pch.h"

using namespace DirectX;

namespace Renderer
{
	class Light
	{
	public:
		enum LightType
		{
			AMBIENT,
			DIRECTIONAL,
			POINT,
			SPOT,
			AREA
		};

		LightType Type;
		XMVECTOR Position;
		XMVECTOR Diffuse;
		XMVECTOR Ambient;
		XMVECTOR Specular;
	};
}