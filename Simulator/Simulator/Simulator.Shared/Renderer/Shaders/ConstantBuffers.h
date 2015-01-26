#pragma once

#include "pch.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;

namespace Renderer
{
	struct DefaultCBuffer
	{
		XMMATRIX matWVP;
		XMMATRIX matRotation;
		XMVECTOR DiffuseVector;
		XMVECTOR DiffuseColor;
		XMVECTOR AmbientColor;
	};

	struct PGM_Pass0_CBuffer
	{
		XMMATRIX matWVP;
		XMMATRIX matRotation;
		XMVECTOR DiffuseVector;
		XMVECTOR DiffuseColor;
		XMVECTOR AmbientColor;
		XMVECTOR CameraPosition;
		XMVECTOR ViewVector;
		XMMATRIX TexTransform;
	};

	struct PGM_Pass1_CBuffer
	{
		XMMATRIX matWVP;
		XMMATRIX matRotation;
		XMVECTOR DiffuseVector;
		XMVECTOR DiffuseColor;
		XMVECTOR AmbientColor;
		XMVECTOR CameraPosition;
		XMVECTOR ViewVector;
		XMMATRIX TexTransform;
	};
}