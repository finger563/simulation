//***************************************************************************************
// RenderStates.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "RenderStates.h"

ID3D11RasterizerState* RenderStates::WireframeRS = 0;
ID3D11RasterizerState* RenderStates::NoCullRS    = 0;
ID3D11RasterizerState* RenderStates::CullFrontRS = 0;
ID3D11RasterizerState* RenderStates::ReverseWindingRS = 0;
	 
ID3D11BlendState*      RenderStates::AlphaToCoverageBS = 0;
ID3D11BlendState*      RenderStates::TransparentBS     = 0;

ID3D11DepthStencilState* RenderStates::DisableDepthDSS = 0;
ID3D11DepthStencilState* RenderStates::DontWriteDepthDSS = 0;
ID3D11DepthStencilState* RenderStates::DepthMaskOffDSS = 0;
ID3D11DepthStencilState* RenderStates::DepthMaskOnDSS = 0;

ID3D11DepthStencilState* RenderStates::PlanetDSS = 0;
ID3D11DepthStencilState* RenderStates::CloudDSS = 0;

void RenderStates::InitAll(ID3D11Device* device)
{
	//
	// WireframeRS
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&noCullDesc, &NoCullRS));

	//
	// CullFront
	//
	D3D11_RASTERIZER_DESC cullFrontDesc;
	ZeroMemory(&cullFrontDesc, sizeof(D3D11_RASTERIZER_DESC));
	cullFrontDesc.FillMode = D3D11_FILL_SOLID;
	cullFrontDesc.CullMode = D3D11_CULL_FRONT;
	cullFrontDesc.FrontCounterClockwise = false;
	cullFrontDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&cullFrontDesc, &CullFrontRS));

	//
	// ReverseWinding
	//
	D3D11_RASTERIZER_DESC reverseWindingDesc;
	ZeroMemory(&reverseWindingDesc, sizeof(D3D11_RASTERIZER_DESC));
	reverseWindingDesc.FillMode = D3D11_FILL_SOLID;
	reverseWindingDesc.CullMode = D3D11_CULL_BACK;
	reverseWindingDesc.FrontCounterClockwise = true;
	reverseWindingDesc.DepthClipEnable = true;

	HR(device->CreateRasterizerState(&reverseWindingDesc, &ReverseWindingRS));

	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC alphaToCoverageDesc = {0};
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

	//
	// TransparentBS
	//

	D3D11_BLEND_DESC transparentDesc = {0};
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend       = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend      = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp        = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha  = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha   = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(device->CreateBlendState(&transparentDesc, &TransparentBS));

	//
	// DisableDepthDSS
	//

	D3D11_DEPTH_STENCIL_DESC disableDepthDesc = {0};
	disableDepthDesc.DepthEnable = false;
	disableDepthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	HR(device->CreateDepthStencilState(&disableDepthDesc, &DisableDepthDSS));

	//
	// DontWriteDepthDSS
	//

	D3D11_DEPTH_STENCIL_DESC dontWriteDepthDesc = {0};
	dontWriteDepthDesc.DepthEnable = false;
	dontWriteDepthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	HR(device->CreateDepthStencilState(&dontWriteDepthDesc, &DontWriteDepthDSS));

	//
	// DepthStencilDSS
	//

	D3D11_DEPTH_STENCIL_DESC depthMaskOffDesc = {0};
	//depthMaskOffDesc.StencilEnable = true;
	depthMaskOffDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	HR(device->CreateDepthStencilState(&depthMaskOffDesc, &DepthMaskOffDSS));

	//
	// DepthStencilDSS
	//

	D3D11_DEPTH_STENCIL_DESC depthMaskOnDesc = {0};
	//depthMaskOnDesc.DepthEnable = true;
	//depthMaskOffDesc.StencilEnable = true;
	depthMaskOnDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;

	HR(device->CreateDepthStencilState(&depthMaskOnDesc, &DepthMaskOnDSS));

	//
	// PlanetDSS
	//

	D3D11_DEPTH_STENCIL_DESC planetDesc = {0};
	planetDesc.DepthEnable = true;
	planetDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	planetDesc.DepthFunc = D3D11_COMPARISON_LESS;
	planetDesc.StencilEnable = true;
	planetDesc.StencilReadMask = 0xff;
	planetDesc.StencilWriteMask = 0xff;

	planetDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	planetDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	planetDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;
	planetDesc.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;
	
	planetDesc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	planetDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	planetDesc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;
	planetDesc.BackFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;

	HR(device->CreateDepthStencilState(&planetDesc, &PlanetDSS));

	//
	// CloudDSS
	//

	D3D11_DEPTH_STENCIL_DESC cloudDesc = {0};
	cloudDesc.DepthEnable = true;
	cloudDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	cloudDesc.DepthFunc = D3D11_COMPARISON_LESS;
	cloudDesc.StencilEnable = true;
	cloudDesc.StencilReadMask = 0xff;
	cloudDesc.StencilWriteMask = 0xff;

	cloudDesc.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	cloudDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	cloudDesc.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;
	cloudDesc.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;
	
	cloudDesc.BackFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;
	cloudDesc.BackFace.StencilDepthFailOp	= D3D11_STENCIL_OP_KEEP;
	cloudDesc.BackFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;
	cloudDesc.BackFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;

	HR(device->CreateDepthStencilState(&cloudDesc, &CloudDSS));
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(WireframeRS);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(CullFrontRS);
	ReleaseCOM(AlphaToCoverageBS);
	ReleaseCOM(TransparentBS);
	ReleaseCOM(DontWriteDepthDSS);
	ReleaseCOM(DisableDepthDSS);
	ReleaseCOM(DepthMaskOffDSS);
	ReleaseCOM(DepthMaskOnDSS);
	
	ReleaseCOM(PlanetDSS);
	ReleaseCOM(CloudDSS);
}