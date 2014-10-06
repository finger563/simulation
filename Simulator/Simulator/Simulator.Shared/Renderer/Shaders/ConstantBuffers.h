#pragma once

namespace Renderer
{
	struct CBuffer
	{
		XMMATRIX matWVP;
		XMMATRIX matRotation;
		XMVECTOR DiffuseVector;
		XMVECTOR DiffuseColor;
		XMVECTOR AmbientColor;
	};
}