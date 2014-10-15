#include "pch.h"

#include "Vector.h"


Base::Math::Vector Base::Math::VectorInit(std::initializer_list<double> l)
{
	double tmp[4] = { 0, 0, 0, 0 };
	if (l.size() <= 4)
	{
		int n = 0;
		for (const double* it = begin(l); it != end(l); ++it, ++n)
		{
			tmp[n] = *it;
		}
	}
	return DirectX::XMVectorSet((float)tmp[0], (float)tmp[1], (float)tmp[2], (float)tmp[3]);
}