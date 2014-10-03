#pragma once

#include "Base\Math\Vector.h"

namespace Renderer
{
	template<typename T>
	struct BaseVertex
	{
		T x, y, z;
		T r, g, b;
	};
}