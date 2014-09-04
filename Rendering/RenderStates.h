//***************************************************************************************
// RenderStates.h by Frank Luna (C) 2011 All Rights Reserved.
//   
// Defines render state objects.  
//***************************************************************************************

#ifndef RENDERSTATES_H
#define RENDERSTATES_H

#include "d3dUtil.h"

class RenderStates
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* NoCullRS;

	static ID3D11RasterizerState* CullFrontRS;
	static ID3D11RasterizerState* ReverseWindingRS;
	 
	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;

	static ID3D11DepthStencilState* DisableDepthDSS;
	static ID3D11DepthStencilState* DontWriteDepthDSS;
	static ID3D11DepthStencilState* DepthMaskOffDSS;
	static ID3D11DepthStencilState* DepthMaskOnDSS;

	static ID3D11DepthStencilState* PlanetDSS;
	static ID3D11DepthStencilState* CloudDSS;
};

#endif // RENDERSTATES_H