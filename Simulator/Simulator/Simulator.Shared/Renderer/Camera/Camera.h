#pragma once

#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"

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
	};
}