#pragma once

#include "GameObject.h"
#include "..\Data Structures\QuadTree.h"

namespace Base
{
	namespace Objects
	{
		template<typename T>
		struct Ellipsoid : GameObject<T>
		{
			Vector<3, T>					radius;				// c is towards poles and is therefore y dimension
			Vector<3, T>					radiusSquared;		// c is towards poles and is therefore y dimension
			DataStructures::QuadTreeNode*	rootQT;				// QuadTree for surface mesh : DO WE WANT TO PUT THIS HERE?
											// OR HOW TO SEPARATE THIS ELLIPSOID CLASS FROM RENDERER SPECIFIC STUFF?

			Ellipsoid();
			Ellipsoid(T a, T b, T c);	// set the three radii
			~Ellipsoid();

			static Vector<3, T> geodeticToNormal(Vector<3, T> geo);
			static Vector<3, T> normalToTangent(Vector<3, T> normal);
			static Vector<3, T> normalToSurface(Vector<3, T> normal);
			static Vector<2, T> geodeticToTexture(Vector<3, T> geo);
		};
	}
}