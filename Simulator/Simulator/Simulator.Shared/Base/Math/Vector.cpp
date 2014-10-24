#include "pch.h"

#include "Vector.h"


Base::Math::Vector Base::Math::VectorInit(std::initializer_list<float> l)
{
	float tmp[4] = { 0, 0, 0, 0 };
	if (l.size() <= 4)
	{
		int n = 0;
		for (const float* it = begin(l); it != end(l); ++it, ++n)
		{
			tmp[n] = *it;
		}
	}
	return DirectX::XMVectorSet(tmp[0], tmp[1], tmp[2], tmp[3]);
}