#pragma once

namespace Base
{
	namespace Math
	{
		typedef DirectX::XMVECTOR Vector;

		Vector VectorInit(std::initializer_list<float> l);

		inline Vector VectorAdd(Vector& a, Vector& b) { return DirectX::operator+(a, b); }
		inline Vector VectorSub(Vector& a, Vector& b) { return DirectX::operator-(a, b); }
		inline Vector VectorDot(Vector& a, Vector& b) { return DirectX::XMVector3Dot(a, b); }
		inline Vector VectorMul(Vector& a, Vector& b) { return DirectX::XMVectorMultiply(a, b); }
		inline Vector VectorMul(Vector& a, float val) { return DirectX::operator*(a, val); }
		inline Vector VectorDiv(Vector& a, float val) { return DirectX::operator/(a, val); }
		inline Vector VectorDiv(Vector& a, Vector& b) { return DirectX::XMVectorDivide(a, b); }
		inline Vector VectorLerp(Vector& a, Vector& b, float t) { return DirectX::XMVectorLerp(a, b, t); }
		inline Vector VectorSize(Vector& a) { return DirectX::XMVector4Length(a); }

		inline void VectorGet(Vector& a, float* val, int index) { DirectX::XMVectorGetByIndexPtr(val, a, index); }
	}
}

//#include "Vector.cpp" // needed to get the definitions of the functions