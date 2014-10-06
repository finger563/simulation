#include "pch.h"

#include "Quaternion.h"

using namespace Base::Math;

template<typename T>
Vector<4, T> QuaternionMultiply(const Vector<4, T>& Q1, const Vector<4, T>& Q2)
{
	Vector<4, T> retVector;
	retVector[3] = (Q1[3] * Q2[3] - Q1[0] * Q2[0] - Q1[1] * Q2[1] - Q1[2] * Q2[2]);	// W
	retVector[0] = (Q1[3] * Q2[0] - Q1[0] * Q2[3] - Q1[1] * Q2[2] - Q1[2] * Q2[1]);	// X
	retVector[1] = (Q1[3] * Q2[1] - Q1[0] * Q2[2] - Q1[1] * Q2[3] - Q1[2] * Q2[0]);	// Y
	retVector[2] = (Q1[3] * Q2[2] - Q1[0] * Q2[1] - Q1[1] * Q2[0] - Q1[2] * Q2[3]);	// Z
	return retVector;
}
template<typename T>
Vector<4, T> QuaternionFromAxisAngle(const Vector<3, T>& Axis, T Angle)
{
	Vector<4, T> retVector;
	retVector[3] = cos(Angle / 2.0);	// W
	T sinHalfAngle = sin(Angle / 2.0);
	retVector[0] = Axis[0] * sinHalfAngle;	// X
	retVector[1] = Axis[1] * sinHalfAngle;	// Y
	retVector[2] = Axis[2] * sinHalfAngle;	// Z
	return retVector;
}

template<typename T>	// assumes a NORMALIZED QUATERNION
Matrix<4, T> RotationMatrixFromQuaternion(const Vector<4, T>& Q)
{
	Matrix<4, T> retMatrix;
	T X = Q[0];
	T Y = Q[1];
	T Z = Q[2];
	T W = Q[3];
	T X2 = X * X;
	T Y2 = Y * Y;
	T Z2 = Z * Z;
	retMatrix[0] = 1.0 - 2.0*Y2 - 2.0*Z2;
	retMatrix[1] = 2.0*X*Y - 2.0*W*Z;
	retMatrix[2] = 2.0*X*Z + 2.0*W*Y;
	retMatrix[3] = 0.0;

	retMatrix[4] = 2.0*X*Y + 2.0*W*Z;
	retMatrix[5] = 1.0 - 2.0*X2 - 2.0*Z2;
	retMatrix[6] = 2.0*Y*Z + 2.0*W*X;
	retMatrix[7] = 0.0;

	retMatrix[8] = 2.0*X*Z - 2.0*W*Y;
	retMatrix[9] = 2.0*Y*Z - 2.0*W*X;
	retMatrix[10] = 1.0 - 2.0*X2 - 2.0*Y2;
	retMatrix[11] = 0.0;

	retMatrix[12] = 0.0;
	retMatrix[13] = 0.0;
	retMatrix[14] = 0.0;
	retMatrix[15] = 1.0;
	return retMatrix;
}