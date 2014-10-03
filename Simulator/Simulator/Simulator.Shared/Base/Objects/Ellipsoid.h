#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include "GameObject.h"

namespace Base
{
	namespace Objects
	{
		template<typename T>
		struct Ellipsoid : GameObject<T>
		{
			Vector<3, T>						radius;				// c is towards poles and is therefore y dimension
			Vector<3, T>						radiusSquared;		// c is towards poles and is therefore y dimension

			Ellipsoid();
			Ellipsoid(T a, T b, T c);	// set the three radii
			~Ellipsoid();

			static Vector<3, T> geodeticToNormal(Vector<3, T> geo);
			static Vector<3, T> normalToTangent(Vector<3, T> normal);
			static Vector<3, T> normalToSurface(Vector<3, T> normal);
			static Vector<2, T> geodeticToTexture(Vector<3, T> geo);
		};

		template<typename T>
		Vector<3, T>
			Ellipsoid<T>::geodeticToNormal(Vector<3, T> geo)
		{
			T cosLat = cos(geo[1]);
			Vector<3, T> normal({
				cosLat * cos(geo[0]),
				sin(geo.vals[1]),
				cosLat * sin(geo[0])
			});
			return normal.Normalized();
		}

		template<typename T>
		Vector<3, T>
			Ellipsoid<T>::normalToTangent(Vector<3, T> normal)
		{
			Vector<3, T> tangent({
				normal[0],
				0,
				normal[2]
			});
			// NEED TO GET ROTATION WORKED OUT: ROTATE THIS BY PI/2 Y-AXIS
			return tangent.Normalized();
		}

		template<typename T>
		Vector<3, T>
			Ellipsoid<T>::normalToSurface(Vector<3, T> normal)
		{
			Vector<3, T> k({
				radius2[0] * normal[0],
				radius2[1] * normal[1],
				radius2[2] * normal[2]
			});
			T gamma = sqrt(
				k[0] * n[0] +
				k[1] * n[1] +
				k[2] * n[2]
				);
			return Vector<3, T>({
				k[0] / gamma,
				k[1] / gamma,
				k[2] / gamma
			});
		}

		template<typename T>
		Vector<2, T>
			Ellipsoid<T>::geodeticToTexture(Vector<3, T> geo)
		{
			return Vector<2, T>({
				geo[0] / (2.0 * M_PI),
				1.0 - (geo[1] / (M_PI) + 0.5)
			});
		}
	}
}
