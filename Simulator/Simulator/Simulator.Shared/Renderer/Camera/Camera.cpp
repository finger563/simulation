#include "pch.h"
#include "Camera.h"

using namespace Renderer;

void Camera::Set(
	XMVECTOR& position,
	XMVECTOR& view,
	XMVECTOR& up,
	float fovY,
	float aspectRatio,
	float nearPlane,
	float farPlane)
{
	Position = position;
	View = view;
	Up = up;
	Right = XMVector3Cross(-View, Up);
	FoVY = fovY;
	Aspect = aspectRatio;
	NearPlane = nearPlane;
	FarPlane = farPlane;
	OrientMatrix = XMMatrixIdentity();
	UpdateCorners();
}

void Camera::Set(
	XMVECTOR& position,
	XMVECTOR& topLeft,
	XMVECTOR& topRight,
	XMVECTOR& bottomLeft,
	XMVECTOR& bottomRight
	)
{
	TopLeft = topLeft;
	TopRight = topRight;
	BottomLeft = bottomLeft;
	BottomRight = bottomRight;
	Position = position;
	XMVECTOR v = (topLeft + bottomRight) / 2;
	XMVECTOR top = (topLeft + topRight) / 2;
	XMVECTOR w = XMVector4Length(topRight - topLeft);
	float width;
	XMVectorGetByIndexPtr(&width, w, 0);
	width = width / 2;
	XMVECTOR h = XMVector4Length(topRight - bottomRight);
	float height;
	XMVectorGetByIndexPtr(&height, h, 0);
	height = height / 2;
	XMVECTOR u = top - v;
	Up = XMVector3Normalize(u);
	View = XMVector3Normalize(v);
	Right = XMVector3Cross(-View, Up);
	Aspect = width / height;
	XMVECTOR a = XMVector3AngleBetweenVectors(bottomRight, topRight);
	float angle;
	XMVectorGetByIndexPtr(&angle, a, 0);
	FoVY = XMConvertToDegrees(angle / 2);
}

void Camera::UpdateCorners()
{
	float FoVX = FoVY * Aspect;  // aspect = x/y
	float width = NearPlane * tan(FoVX);
	float height = NearPlane * tan(FoVY);
	XMVECTOR r = Right * width;
	XMVECTOR u = Up * height;
	XMVECTOR v = View * NearPlane;
	TopLeft = XMVector3Normalize(v + u - r);
	TopRight = XMVector3Normalize(v + u + r);
	BottomLeft = XMVector3Normalize(v - u - r);
	BottomRight = XMVector3Normalize(v - u + r);
}

void Camera::Strafe(float Dist)
{
	Position = Position + Right * Dist;
}

void Camera::Walk(float Dist)
{
	Position = Position + View * Dist;
}

void Camera::Pitch(float Angle)
{
	RotateAroundRight(Angle);
}

void Camera::Roll(float Angle)
{
	RotateAroundView(Angle);
}

void Camera::Yaw(float Angle)
{
	RotateAroundUp(Angle);
}

void Camera::UpdateMatrices()
{
	ViewMatrix = XMMatrixLookToLH(Position, View, Up);
	ProjMatrix = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(FoVY),
		Aspect,
		NearPlane,
		FarPlane
		);
	UpdateCorners();
}

void Camera::RotateAroundPoint(XMVECTOR& Point, XMVECTOR& Axis, float Angle)
{
	XMVECTOR offset = Position - Point;
	XMVECTOR quatRot = XMQuaternionRotationAxis(Axis, Angle);
	offset = XMVector3Rotate(offset, quatRot);
	Position = Point + offset;
}

void Camera::RotateByAxisAngle(XMVECTOR& Axis, float Angle)
{
	XMVECTOR quatRot = XMQuaternionRotationAxis(Axis, Angle);
	View = XMVector3Normalize(XMVector3Rotate(View, quatRot));
	Right = XMVector3Normalize(XMVector3Rotate(Right, quatRot));
	Up = XMVector3Normalize(XMVector3Rotate(Up, quatRot));
}

void Camera::RotateAroundUp(float Angle)
{
	XMMATRIX R = XMMatrixRotationAxis(Up, Angle);
	Right = XMVector3TransformNormal(Right, R);
	Up = XMVector3TransformNormal(Up, R);
	View = XMVector3TransformNormal(View, R);
}

void Camera::RotateAroundRight(float Angle)
{
	XMMATRIX R = XMMatrixRotationAxis(Right, Angle);
	Right = XMVector3TransformNormal(Right, R);
	Up = XMVector3TransformNormal(Up, R);
	View = XMVector3TransformNormal(View, R);
}

void Camera::RotateAroundView(float Angle)
{
	XMMATRIX R = XMMatrixRotationAxis(View, Angle);
	Right = XMVector3TransformNormal(Right, R);
	Up = XMVector3TransformNormal(Up, R);
	View = XMVector3TransformNormal(View, R);
}