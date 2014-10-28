#include "pch.h"
#include "Mesh.h"
#include "Base\Math\Vector.h"


namespace Base
{
	Vertex LinearInterpolate(Vertex& v0, Vertex& v1, float t)
	{
		Vertex retVert(v0);
		Base::Math::Vector pos0 = Base::Math::VectorInit({ v0.pos[0], v0.pos[1], v0.pos[2] });
		Base::Math::Vector pos1 = Base::Math::VectorInit({ v1.pos[0], v1.pos[1], v1.pos[2] });
		Base::Math::Vector newPos = Base::Math::VectorLerp(pos0, pos1, t);
		for (int i = 0; i < 3; i++)
			Base::Math::VectorGet(newPos, &retVert.pos[i], i);
		return retVert;
	}

	Vertex SphericalInterpolate(Vertex& v0, Vertex& v1, float t,
		float r, float x, float y, float z)
	{
		return EllipsoidInterpolate(v0, v1, t, r, r, r, x, y, z);
	}

	Vertex EllipsoidInterpolate(Vertex& v0, Vertex& v1, float t,
		float a, float b, float c, float x, float y, float z)
	{
		float pos[3] = { x, y, z };
		float radius[3] = { a, b, c };
		Vertex retVert(LinearInterpolate(v0, v1, t));
		Base::Math::Vector nPos = Base::Math::VectorInit({ retVert.pos[0] - x, retVert.pos[1] - y, retVert.pos[2] - z });
		nPos = Base::Math::VectorNorm(nPos);
		for (int i = 0; i < 3; i++)
		{
			Base::Math::VectorGet(nPos, &retVert.pos[i], i);		// update the position
			retVert.pos[i] = retVert.pos[i] * radius[i] + pos[i];
			Base::Math::VectorGet(nPos, &retVert.normal[i], i);		// update the normal
		}
		return retVert;
	}
}