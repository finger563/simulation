#pragma once

#include "pch.h"

using namespace DirectX;

namespace Renderer
{
	struct Light
	{
		XMVECTOR Position;
		XMVECTOR Diffuse;
		XMVECTOR Ambient;
		XMVECTOR Specular;
	};

	struct DirectionalLight : Light
	{
		// same as Light
	};

	struct PointLight : Light
	{
		XMVECTOR Attenuation;
		float Range;
	};

	struct SpotLight : PointLight
	{
		XMVECTOR Direction;
		float Spot;
	};
}