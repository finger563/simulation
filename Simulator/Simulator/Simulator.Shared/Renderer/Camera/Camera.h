#pragma once

#include "pch.h"
#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"

using namespace DirectX;

namespace Renderer
{
	template<typename T>
	class Camera
	{
	public:
		Vector<3, T> Position;
		Vector<4, T> Orientation;
		T FoVY;
		T Aspect;
		T NearPlane;
		T FarPlane;

		inline XMVECTOR PositionXM()
		{
			XMVECTOR retVec = XMVectorSet(Position[0], Position[1], Position[2], 0);
			return retVec;
		}
		inline XMVECTOR LookAtXM()
		{
			XMVECTOR retVec = XMVectorSet(0, 0, 0, 0);
			return retVec;
		}
		inline XMVECTOR UpXM()
		{
			XMVECTOR retVec = XMVectorSet(0, 1, 0, 0);
			return retVec;
		}
	};
}