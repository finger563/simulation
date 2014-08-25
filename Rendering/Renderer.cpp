//***************************************************************************************
// Renderer.cpp
//***************************************************************************************

#include "d3dx11Effect.h"
#include "GeometryGenerator.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "Renderer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	Renderer theApp(hInstance);
	
	if( !theApp.Init() )
		return 0;
	
	return theApp.Run();
}
 

Renderer::Renderer(HINSTANCE hInstance)
: D3DApp(hInstance), mSky(0), mBoxVB(0), mBoxIB(0), mDiffuseMapSRV(0), mEarthNormalTexSRV(0), control(hInstance),  mRenderOptions(RenderOptionsNormalMap)
{
	mMainWndCaption = L"Crate Demo";
	
	XMMATRIX I = XMMatrixIdentity();
	XMStoreFloat4x4(&mBoxWorld, I);
	XMStoreFloat4x4(&mTexTransform, I);
	XMStoreFloat4x4(&mProj, I);
	
	mDirLights[0].Ambient  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[0].Diffuse  = XMFLOAT4(0.7f, 0.7f, 0.6f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.707f, 0.0f, 0.707f);

	mDirLights[1].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse  = XMFLOAT4(0.40f, 0.40f, 0.40f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(0.0f, -0.707f, 0.707f);

	mDirLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Direction = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);

	mBoxMat.Ambient  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mBoxMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mBoxMat.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
	mBoxMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

Renderer::~Renderer()
{
	SafeDelete(mSky);

	ReleaseCOM(mBoxVB);
	ReleaseCOM(mBoxIB);
	ReleaseCOM(mDiffuseMapSRV);
	ReleaseCOM(mEarthNormalTexSRV);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

bool Renderer::Init()
{
	control.Init();
	if(!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);

	mSky = new Sky(md3dDevice, L"Textures/snowcube1024.dds", 10.0f);
	
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/earthmap.dds",0, 0, &mDiffuseMapSRV, 0 ));
 
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/earth_nmap.dds", 0, 0, &mEarthNormalTexSRV, 0 ));

	BuildGeometryBuffers();

	return true;
}

void Renderer::OnResize()
{
	D3DApp::OnResize();

	control.get_Camera().SetLens(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, control.get_farClipPlaneDist());
}

void Renderer::UpdateScene(float dt)
{
	XMVECTOR vRadius = XMVector3Length(control.get_Camera().GetPositionXM());
	XMFLOAT3 fRadius;
	XMStoreFloat3(&fRadius,vRadius);
	float radius = fRadius.x;
	// Restrict the radius.
	radius = MathHelper::Clamp(radius, control.get_earthRadius(), control.get_maxRadius());

	// Camera control:
	if (GetAsyncKeyState('W') & 0x8000)
		control.get_Camera().Walk(dt*(radius-control.get_earthRadius()));
	
	if (GetAsyncKeyState('S') & 0x8000)
		control.get_Camera().Walk(-dt*(radius-control.get_earthRadius()));

	if (GetAsyncKeyState('A') & 0x8000)
		control.get_Camera().Strafe(-dt*(radius-control.get_earthRadius()));

	if (GetAsyncKeyState('D') & 0x8000)
		control.get_Camera().Strafe(dt*(radius-control.get_earthRadius()));
	
	//
	// Switch the rendering effect based on key presses.
	//
	if( GetAsyncKeyState('2') & 0x8000 )
		mRenderOptions = RenderOptionsBasic; 

	if( GetAsyncKeyState('3') & 0x8000 )
		mRenderOptions = RenderOptionsNormalMap; 

	if( GetAsyncKeyState('4') & 0x8000 )
		mRenderOptions = RenderOptionsDisplacementMap; 
}

void Renderer::DrawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	  
	control.get_Camera().UpdateViewMatrix();
	XMMATRIX view		= control.get_Camera().View();
	XMMATRIX proj		= control.get_Camera().Proj();
	XMMATRIX viewProj	= control.get_Camera().ViewProj();
	
	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};

	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(control.get_Camera().GetPosition());
	Effects::BasicFX->SetCubeMap(mSky->CubeMapSRV());

	Effects::NormalMapFX->SetDirLights(mDirLights);
	Effects::NormalMapFX->SetEyePosW(control.get_Camera().GetPosition());
	Effects::NormalMapFX->SetCubeMap(mSky->CubeMapSRV());

	// Set per frame constants.
	Effects::DisplacementMapFX->SetDirLights(mDirLights);
	Effects::DisplacementMapFX->SetEyePosW(control.get_Camera().GetPosition());
	Effects::DisplacementMapFX->SetCubeMap(mSky->CubeMapSRV());

	// These properties could be set per object if needed.
	Effects::DisplacementMapFX->SetHeightScale(1000.0f);
	Effects::DisplacementMapFX->SetMaxTessDistance(1.0f);
	Effects::DisplacementMapFX->SetMinTessDistance(1000.0f);
	Effects::DisplacementMapFX->SetMinTessFactor(1.0f);
	Effects::DisplacementMapFX->SetMaxTessFactor(8.0f);
 
	ID3DX11EffectTechnique* activeTech       = Effects::DisplacementMapFX->Light3TexTech;
	switch(mRenderOptions)
	{
	case RenderOptionsBasic:
		activeTech = Effects::BasicFX->Light3TexTech;
		md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case RenderOptionsNormalMap:
		activeTech = Effects::NormalMapFX->Light3TexTech;
		md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	case RenderOptionsDisplacementMap:
		activeTech = Effects::DisplacementMapFX->Light3TexTech;
		md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		break;
	}
	
	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;
	
	UINT stride = sizeof(Vertex::PosNormalTexTan);
    UINT offset = 0;

	md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTexTan);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);

	if( GetAsyncKeyState('1') & 0x8000 )
		md3dImmediateContext->RSSetState(RenderStates::WireframeRS);

    D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		// Draw the box.
		world = XMLoadFloat4x4(&mBoxWorld);
		worldInvTranspose = MathHelper::InverseTranspose(world);
		worldViewProj = world*view*proj;

		switch(mRenderOptions)
		{
		case RenderOptionsBasic:
			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(XMMatrixScaling(8.0f, 10.0f, 1.0f));
			Effects::BasicFX->SetMaterial(mBoxMat);
			Effects::BasicFX->SetDiffuseMap(mDiffuseMapSRV);
			break;
		case RenderOptionsNormalMap:
			Effects::NormalMapFX->SetWorld(world);
			Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
			Effects::NormalMapFX->SetTexTransform(XMMatrixScaling(8.0f, 10.0f, 1.0f));
			Effects::NormalMapFX->SetMaterial(mBoxMat);
			Effects::NormalMapFX->SetDiffuseMap(mDiffuseMapSRV);
			Effects::NormalMapFX->SetNormalMap(mEarthNormalTexSRV);
			break;
		case RenderOptionsDisplacementMap:
			Effects::DisplacementMapFX->SetWorld(world);
			Effects::DisplacementMapFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::DisplacementMapFX->SetViewProj(viewProj);
			Effects::DisplacementMapFX->SetWorldViewProj(worldViewProj);
			Effects::DisplacementMapFX->SetTexTransform(XMMatrixScaling(8.0f, 10.0f, 1.0f));
			Effects::DisplacementMapFX->SetMaterial(mBoxMat);
			Effects::DisplacementMapFX->SetDiffuseMap(mDiffuseMapSRV);
			Effects::DisplacementMapFX->SetNormalMap(mEarthNormalTexSRV);
			break;
		}
		
		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mBoxIndexCount, mBoxIndexOffset, mBoxVertexOffset);
    }

	// FX sets tessellation stages, but it does not disable them.  So do that here
	// to turn off tessellation.
	md3dImmediateContext->HSSetShader(0, 0, 0);
	md3dImmediateContext->DSSetShader(0, 0, 0);
	
	// restore default states, as the SkyFX changes them in the effect file.
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);

	HR(mSwapChain->Present(0, 0));
}

void Renderer::BuildGeometryBuffers()
{
	GeometryGenerator::MeshData box;

	GeometryGenerator geoGen;

	geoGen.CreateGeosphere(control.get_earthRadius(),600.0f,box);

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mBoxVertexOffset      = 0;

	// Cache the index count of each object.
	mBoxIndexCount      = box.Indices.size();

	// Cache the starting index for each object in the concatenated index buffer.
	mBoxIndexOffset      = 0;
	
	UINT totalVertexCount = box.Vertices.size();

	UINT totalIndexCount = mBoxIndexCount;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<Vertex::PosNormalTexTan> vertices(totalVertexCount);

	UINT k = 0;
	for(size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos			= box.Vertices[i].Position;
		vertices[k].Normal		= box.Vertices[i].Normal;
		vertices[k].Tex			= box.Vertices[i].TexC;
		vertices[k].TangentU	= box.Vertices[i].TangentU;
	}

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan) * totalVertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mBoxVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<UINT> indices;
	indices.insert(indices.end(), box.Indices.begin(), box.Indices.end());

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mBoxIB));
}
 
