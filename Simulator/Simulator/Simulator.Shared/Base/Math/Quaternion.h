#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace Base
{
	namespace Math
	{
		template<typename T>
		Vector<4, T> QuaternionMultiply(const Vector<4, T>& Q1, const Vector<4, T>& Q2);
		template<typename T>	// assumes a NORMALIZED AXIS VECTOR
		Vector<4, T> QuaternionFromAxisAngle(const Vector<3, T>& Axis, T Angle);

		template<typename T>	// assumes a NORMALIZED QUATERNION
		Matrix<4,T> RotationMatrixFromQuaternion(const Vector<4, T>& Q);
	}
}

#include "Quaternion.cpp" // needed to get the definitions of the functions