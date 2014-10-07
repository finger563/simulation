#pragma once

#include "pch.h"

using namespace DirectX;

namespace Renderer
{
	class Camera
	{
	public:
		XMVECTOR Position;
		XMVECTOR View;
		XMVECTOR Up;
		XMVECTOR Orientation;
		float FoVY;
		float Aspect;
		float NearPlane;
		float FarPlane;

		void RotateByAxisAngle(XMVECTOR& Axis, float Angle)
		{
			XMMATRIX matRot = XMMatrixRotationAxis(Axis, Angle);
			View = XMVector3Transform(View, matRot);
			Position = XMVector3Transform(Position, matRot);
		}
	};
}

//#include "Camera.cpp"