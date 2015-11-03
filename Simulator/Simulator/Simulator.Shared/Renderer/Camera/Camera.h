#pragma once

#include "pch.h"

#include "Base\Math\Vector.h"
#include "..\Device Resources\DeviceResources.h"

using namespace DirectX;

namespace Renderer
{
	class Camera
	{
	public:
		struct FrustumVertex {
			float x, y, z, w;
		};

		XMVECTOR Position;
		XMVECTOR View;
		XMVECTOR Up;
		XMVECTOR Right;
		XMVECTOR Orientation;
		XMVECTOR TopLeft, TopRight, BottomLeft, BottomRight; // corner vectors for frustum
		float FoVY;
		float Aspect;
		float NearPlane;
		float FarPlane;

		XMMATRIX ViewMatrix;		// matrix used to store the view of the camera
		XMMATRIX ProjMatrix;		// matrix used to store the projection for the camera
		XMMATRIX OrientMatrix;		// matrix used to store the orientation of the screen

		void Set(
			XMVECTOR& position,
			XMVECTOR& view,
			XMVECTOR& up,
			float fovY,
			float aspectRatio,
			float nearPlane,
			float farPlane);

		void Set(const Camera& c);

		void UpdateCorners();
		void Set(
			XMVECTOR& position,
			XMVECTOR& topLeft,
			XMVECTOR& topRight,
			XMVECTOR& bottomLeft,
			XMVECTOR& bottomRight
			);
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

		int numIndices;
		ComPtr<ID3D11Buffer> frustumvertexbuffer;				// Dx11.2 GPU vertex buffer interface
		ComPtr<ID3D11Buffer> frustumindexbuffer;				// Dx11.2 GPU index buffer interface
		void MakeFrustumBuffers(std::shared_ptr<DeviceResources>& deviceResources);  // make vertex and index buffers for drawing frustum
	};
}
