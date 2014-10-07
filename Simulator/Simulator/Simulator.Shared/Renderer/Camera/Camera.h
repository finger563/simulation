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

		void UpdateMatrices();			// called in Renderer::Render() before sending matrices to GPU

		void Strafe(float Dist);		// + = right, - = left
		void Walk(float Dist);			// + = forward, - = back

		void RotateAroundPoint(XMVECTOR& Point, XMVECTOR& Axis, float Angle);
		void RotateByAxisAngle(XMVECTOR& Axis, float Angle);
		void RotateAroundUp(float Angle);				// + = CW, - = CCW
		void RotateAroundRight(float Angle);			// + = down, - = up
		void RotateAroundView(float Angle);				
	};
}
