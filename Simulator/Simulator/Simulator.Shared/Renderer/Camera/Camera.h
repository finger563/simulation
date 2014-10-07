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
		XMVECTOR Right;
		XMVECTOR Orientation;
		float FoVY;
		float Aspect;
		float NearPlane;
		float FarPlane;

		XMMATRIX ViewMatrix;
		XMMATRIX ProjMatrix;

		void UpdateMatrices()
		{
			ViewMatrix = XMMatrixLookToLH(Position, View, Up);
			ProjMatrix = XMMatrixPerspectiveFovLH(
				XMConvertToRadians(FoVY),
				Aspect,
				NearPlane,
				FarPlane
				);
		}

		void RotateAroundPoint(XMVECTOR& Point, XMVECTOR& Axis, float Angle)
		{
			XMVECTOR offset = Position - Point;
			XMVECTOR quatRot = XMQuaternionRotationAxis(Axis, Angle);
			offset = XMVector3Rotate(offset, quatRot);
			Position = Point + offset;
		}

		void RotateByAxisAngle(XMVECTOR& Axis, float Angle)
		{
			XMVECTOR quatRot = XMQuaternionRotationAxis(Axis, Angle);
			View = XMVector3Rotate(View, quatRot);
			Right = XMVector3Rotate(Right, quatRot);
			Up = XMVector3Rotate(Up, quatRot);
		}

		void RotateAroundUp(float Angle)
		{
			XMVECTOR quatRot = XMQuaternionRotationAxis(Up, Angle);
			View = XMVector3Rotate(View, quatRot);
			Right = XMVector3Cross(View, Up);
		}

		void RotateAroundRight(float Angle)
		{
			XMVECTOR quatRot = XMQuaternionRotationAxis(Right, Angle);
			View = XMVector3Rotate(View, quatRot);
			Up = XMVector3Cross(Right, View);
		}
	};
}

//#include "Camera.cpp"