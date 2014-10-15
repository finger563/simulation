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

		XMMATRIX ViewMatrix;		// matrix used to store the view of the camera
		XMMATRIX ProjMatrix;		// matrix used to store the projection for the camera
		XMMATRIX OrientMatrix;		// matrix used to store the orientation of the screen

		void Set(
			XMVECTOR position,
			XMVECTOR view,
			XMVECTOR up,
			float fovY,
			float aspectRatio,
			float nearPlane,
			float farPlane);

		void UpdateMatrices();			// called in Renderer::Render() before sending matrices to GPU

		void Strafe(float Dist);		// + = right, - = left
		void Walk(float Dist);			// + = forward, - = back
		void Pitch(float Angle);
		void Roll(float Angle);
		void Yaw(float Angle);

		void RotateAroundPoint(XMVECTOR& Point, XMVECTOR& Axis, float Angle);
		void RotateByAxisAngle(XMVECTOR& Axis, float Angle);
		void RotateAroundUp(float Angle);				// + = CW, - = CCW
		void RotateAroundRight(float Angle);			// + = down, - = up
		void RotateAroundView(float Angle);		
	};
}
