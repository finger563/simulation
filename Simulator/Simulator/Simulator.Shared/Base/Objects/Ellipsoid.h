#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "GameObject.h"

namespace Base
{
	namespace Objects
	{
		struct Ellipsoid : GameObject
		{
			Vector						radius;				// c is towards poles and is therefore y dimension
			Vector						radiusSquared;		// c is towards poles and is therefore y dimension

			Ellipsoid();
			Ellipsoid(float a, float b, float c);	// set the three radii
			~Ellipsoid();

			static Vector geodeticToNormal(Vector& geo);
			static Vector normalToTangent(Vector& normal);
			static Vector normalToSurface(Vector& normal, Vector& radius2);
			static Vector geodeticToTexture(Vector& geo);
		};
	}
}
