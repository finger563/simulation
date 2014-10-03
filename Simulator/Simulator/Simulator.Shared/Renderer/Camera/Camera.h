#pragma once

#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"

namespace Renderer
{
	template<typename T>
	ref class Camera
	{
	public:
		property Vector<3, T> Position;
		property Vector<4, T> Orientation;
	};
}