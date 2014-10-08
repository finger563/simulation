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

		Light()
		{
			Position = Diffuse = Ambient = Specular = XMVectorSet(0, 0, 0, 0);
		}
		Light(XMVECTOR& p, XMVECTOR& d, XMVECTOR& a, XMVECTOR& s)
			: Position(p), Diffuse(d), Ambient(a), Specular(s) {}
	};

	struct DirectionalLight : Light
	{
		// same as Light
		DirectionalLight() : Light() {}
		DirectionalLight(XMVECTOR& p, XMVECTOR& d, XMVECTOR& a, XMVECTOR& s)
			: Light(p, d, a, s) {}
	};

	struct PointLight : Light
	{
		XMVECTOR Attenuation;
		float Range;

		PointLight() : Light(), Attenuation(XMVectorSet(0, 0, 0, 0)), Range(0) {}
		PointLight(XMVECTOR& p, XMVECTOR& d, XMVECTOR& a, XMVECTOR& s, XMVECTOR& att, float r)
			: Light(p, d, a, s), Attenuation(att), Range(r) {}
	};

	struct SpotLight : PointLight
	{
		XMVECTOR Direction;
		float Spot;

		SpotLight() : PointLight(), Direction(XMVectorSet(0, 0, 0, 0)), Spot(0) {}
		SpotLight(XMVECTOR& p, XMVECTOR& d, XMVECTOR& a, XMVECTOR& s, XMVECTOR& att, float r, XMVECTOR& dir, float _spot)
			: PointLight(p, d, a, s, att, r), Direction(dir), Spot(_spot) {}
	};
}