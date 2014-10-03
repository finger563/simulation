#pragma once

#include "Base\Math\Vector.h"

namespace Renderer
{
	template<typename T>
	struct BaseVertex
	{
		Vector<3, T> Position;

		BaseVertex<T>(std::initializer_list<T> l)
		{
			assert(l.size() == 3);
			int n = 0;
			for (const T* it = begin(l); it != end(l); ++it)
			{
				Position[n++] = *it;
			}
		}
	};
}