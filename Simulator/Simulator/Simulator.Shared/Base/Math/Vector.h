#pragma once

namespace Base
{
	namespace Math
	{
		typedef DirectX::XMVECTOR Vector;

		Vector VectorInit(std::initializer_list<double> l);

		inline Vector VectorAdd(Vector& a, Vector& b) { return DirectX::operator+(a, b); }
		inline Vector VectorSub(Vector& a, Vector& b) { return DirectX::operator-(a, b); }
		inline Vector VectorDot(Vector& a, Vector& b) { return DirectX::XMVector3Dot(a, b); }
		inline Vector VectorMul(Vector& a, Vector& b) { return DirectX::XMVectorMultiply(a, b); }
		inline Vector VectorMul(Vector& a, double val) { return DirectX::operator*(a, (float)val); }
		inline Vector VectorDiv(Vector& a, double val) { return DirectX::operator/(a, (float)val); }
		inline Vector VectorDiv(Vector& a, Vector& b) { return DirectX::XMVectorDivide(a, b); }

		// not recommended for performance reasons
		inline double VectorIndex(Vector& a, int val) { return (double)DirectX::XMVectorGetByIndex(a, val); } 
	}
}

//#include "Vector.cpp" // needed to get the definitions of the functions