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
		XMVECTOR CameraPosition;
		XMVECTOR ViewVector;
		XMMATRIX TexTransform;
	};
}