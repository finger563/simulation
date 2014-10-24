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
		Base::Math::VectorGet(newPos, retVert.pos, 3);
		return retVert;
	}

	Vertex SphericalInterpolate(Vertex& v0, Vertex& v1, float t,
		float r, float x, float y, float z)
	{
		Vertex retVert(LinearInterpolate(v0, v1, t));
		Base::Math::Vector nPos = Base::Math::VectorInit({ retVert.pos[0] - x, retVert.pos[1] - y, retVert.pos[2] - z });
		Base::Math::Vector len = Base::Math::VectorSize(nPos);
		nPos = Base::Math::VectorDiv(nPos, len);
		Base::Math::VectorGet(nPos, retVert.pos, 3);
		retVert.pos[0] *= r;
		retVert.pos[0] += x;
		retVert.pos[1] *= r;
		retVert.pos[1] += y;
		retVert.pos[2] *= r;
		retVert.pos[2] += z;
		return retVert;
	}

	Vertex EllipsoidInterpolate(Vertex& v0, Vertex& v1, float t,
		float a, float b, float c, float x, float y, float z)
	{
		Vertex retVert(LinearInterpolate(v0, v1, t));
		Base::Math::Vector nPos = Base::Math::VectorInit({ retVert.pos[0] - x, retVert.pos[1] - y, retVert.pos[2] - z });
		Base::Math::Vector len = Base::Math::VectorSize(nPos);
		nPos = Base::Math::VectorDiv(nPos, len);
		Base::Math::VectorGet(nPos, retVert.pos, 3);
		retVert.pos[0] *= a;
		retVert.pos[0] += x;
		retVert.pos[1] *= b;
		retVert.pos[1] += y;
		retVert.pos[2] *= c;
		retVert.pos[2] += z;
		return retVert;
	}
}