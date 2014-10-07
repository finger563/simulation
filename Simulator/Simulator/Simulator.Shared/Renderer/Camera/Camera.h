#pragma once

#include "pch.h"
#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"
#include "Base\Math\Quaternion.h"

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