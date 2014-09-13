//***************************************************************************************
// Renderer.cpp
//***************************************************************************************

#include "d3dx11Effect.h"
#include "MathHelper.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include "Renderer.h"

#if USE_QUADTREE
#else
#include "GeometryGenerator.h"
#endif

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
: D3DApp(hInstance), mVB(0), mIB(0), mEarthDiffuseMapSRV(0), mEarthNormalTexSRV(0), control(hInstance),  mRenderOptions(RenderOptionsDisplacementMap)
{
	mMainWndCaption = L"Crate Demo";
	control.Init();
	
	XMMATRIX I = XMMatrixIdentity();

	XMVECTOR trans = XMLoadFloat3(&control.get_earthPosW());
	XMMATRIX T = XMMatrixTranslationFromVector(trans);
	
	XMStoreFloat4x4(&mEarthWorld, T);

	XMStoreFloat4x4(&mTexTransform, I);
	XMStoreFloat4x4(&mProj, I);
	
	mDirLights[0].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[0].Diffuse  = XMFLOAT4(0.7f, 0.7f, 0.6f, 1.0f);
	mDirLights[0].Specular = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mDirLights[0].Direction = XMFLOAT3(0.707f, 0.0f, 0.707f);

	mDirLights[1].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[1].Diffuse  = XMFLOAT4(0.40f, 0.40f, 0.40f, 1.0f);
	mDirLights[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[1].Direction = XMFLOAT3(0.0f, -0.707f, 0.707f);

	mDirLights[2].Ambient  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mDirLights[2].Diffuse  = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLights[2].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 2.0f);
	mDirLights[2].Direction = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);

	mEarthMat.Ambient  = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	mEarthMat.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mEarthMat.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mEarthMat.Reflect  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
}

Renderer::~Renderer()
{
	ReleaseCOM(mVB);
	ReleaseCOM(mIB);
	ReleaseCOM(mEarthDiffuseMapSRV);
	ReleaseCOM(mEarthNormalTexSRV);
	ReleaseCOM(mCloudsDiffuseMapSRV);

	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

bool Renderer::Init()
{
	if(!D3DApp::Init())
		return false;

	// Must init Effects first since InputLayouts depend on shader signatures.
	Effects::InitAll(md3dDevice);
	InputLayouts::InitAll(md3dDevice);
	RenderStates::InitAll(md3dDevice);
		
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/earthmap.dds",0, 0, &mEarthDiffuseMapSRV, 0 ));
 
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/earth_nmap.dds", 0, 0, &mEarthNormalTexSRV, 0 ));
	
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/clouds.dds",0, 0, &mCloudsDiffuseMapSRV, 0 ));
	HR(D3DX11CreateShaderResourceViewFromFile(md3dDevice, 
		L"Textures/clouds-normal.dds",0, 0, &mCloudsNormalTexSRV, 0 ));

	BuildGeometryBuffers();

	return true;
}

void Renderer::OnResize()
{
	D3DApp::OnResize();

	control.get_Camera().SetLens(0.25f*MathHelper::Pi, AspectRatio(), control.get_nearClipPlaneDist(), control.get_farClipPlaneDist());
}

void Renderer::UpdateScene(float dt)
{

	if (GetAsyncKeyState('R') & 0x8000) {
		control.set_earthAngle( control.get_earthAngle() + dt );
		XMVECTOR trans = XMLoadFloat3(&control.get_earthPosW());
		XMMATRIX T = XMMatrixTranslationFromVector(trans);
		XMMATRIX R = XMMatrixRotationY(control.get_earthAngle());
	
		XMStoreFloat4x4(&mEarthWorld, R*T);
	}

	if (GetAsyncKeyState('T') & 0x8000) {
		control.set_earthAngle( control.get_earthAngle() - dt );
		XMVECTOR trans = XMLoadFloat3(&control.get_earthPosW());
		XMMATRIX T = XMMatrixTranslationFromVector(trans);
		XMMATRIX R = XMMatrixRotationY(control.get_earthAngle());
	
		XMStoreFloat4x4(&mEarthWorld, R*T);
	}

	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
		control.set_Speed(control.get_Speed()*0.9f);
	if (GetAsyncKeyState(VK_TAB) & 0x8000)
		control.set_Speed(control.get_Speed()*1.1f);
	
	float original_speed = control.get_Speed();
	if (GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		control.set_Speed(control.get_Speed() * 100.0f);

	// Camera control:
	if (GetAsyncKeyState('W') & 0x8000)
		control.get_Camera().Walk(dt*control.get_Speed());
	
	if (GetAsyncKeyState('S') & 0x8000)
		control.get_Camera().Walk(-dt*control.get_Speed());

	if (GetAsyncKeyState('A') & 0x8000)
		control.get_Camera().Strafe(-dt*control.get_Speed());

	if (GetAsyncKeyState('D') & 0x8000)
		control.get_Camera().Strafe(dt*control.get_Speed());
	
	control.set_Speed(original_speed);
	
	if (GetAsyncKeyState('Q') & 0x8000)
		control.get_Camera().RotateLook(dt);
	if (GetAsyncKeyState('E') & 0x8000)
		control.get_Camera().RotateLook(-dt);

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		control.get_Camera().RotateUp(dt);
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		control.get_Camera().RotateUp(-dt);
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		control.get_Camera().Pitch(dt);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		control.get_Camera().Pitch(-dt);
	
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
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
	  
	control.get_Camera().UpdateViewMatrix();
	XMMATRIX view		= control.get_Camera().View();
	XMMATRIX proj		= control.get_Camera().Proj();
	XMMATRIX viewProj	= control.get_Camera().ViewProj();
	
	float blendFactor[] = {0.0f, 0.0f, 0.0f, 0.0f};
	
	ID3DX11EffectTechnique* activeTech;
    D3DX11_TECHNIQUE_DESC techDesc;

	XMMATRIX world;
	XMMATRIX worldInvTranspose;
	XMMATRIX worldViewProj;
	
	world = XMLoadFloat4x4(&mEarthWorld);
	worldInvTranspose = MathHelper::InverseTranspose(world);
	worldViewProj = world*view*proj;
	
	UINT stride = sizeof(Vertex::PosNormalTexTan);
    UINT offset = 0;
	
	
	float wavelength[3] = {0.650f, 0.570f, 0.475f};
	XMFLOAT3 invWaveLength = XMFLOAT3( 1.0f/powf(wavelength[0],4.0f),
									   1.0f/powf(wavelength[1],4.0f),
									   1.0f/powf(wavelength[2],4.0f));

	float outerRadius = control.get_earthRadius() + control.get_skyAltitude();
	float innerRadius = control.get_earthRadius();
	
	XMVECTOR vPlanet = XMLoadFloat3(&control.get_earthPosW());

	XMVECTOR vEye = XMLoadFloat3(&control.get_Camera().GetPosition());
	XMVECTOR relPos = vEye - vPlanet;
	XMFLOAT3 cameraPos;
	XMStoreFloat3(&cameraPos,relPos);
	XMFLOAT3 fRadius;
	XMStoreFloat3(&fRadius, XMVector3Length(relPos));
	float height = fRadius.y;
	float Kr = 0.0025f;
	float Km = 0.0010f;
	float ESun = 20.0f;
	float scale = 1.0f / ( outerRadius - innerRadius );

	XMVECTOR light = XMLoadFloat3(&mDirLights[0].Direction);
	XMFLOAT3 sunPos;
	XMStoreFloat3(&sunPos,-light);


	// Set per frame constants.
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetEyePosW(control.get_Camera().GetPosition());
	
#if USE_QUADTREE

	ID3D11Buffer* mIB_test;
	
	std::vector<UINT> indices = earth.getIndices();

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * indices.size();
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB_test));

	md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTexTan);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIB_test, DXGI_FORMAT_R32_UINT, 0);
	
	md3dImmediateContext->RSSetState(RenderStates::WireframeRS);

	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	Effects::BasicFX->SetWorld(world);
	Effects::BasicFX->SetDirLights(mDirLights);
	Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::BasicFX->SetWorldViewProj(worldViewProj);
	Effects::BasicFX->SetTexTransform(reinterpret_cast<CXMMATRIX>(mTexTransform));
	Effects::BasicFX->SetMaterial(mEarthMat);
	Effects::BasicFX->SetDiffuseMap(mEarthDiffuseMapSRV);
	
	activeTech = Effects::BasicFX->Light1Tech;
	activeTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(indices.size(), 0, 0);
	}

	ReleaseCOM(mIB_test);
#else
	Effects::NormalMapFX->SetDirLights(mDirLights);
	Effects::NormalMapFX->SetEyePosW(control.get_Camera().GetPosition());
	
	// SKY EFFECTS
	Effects::SkyFX->SetEyePosW(control.get_Camera().GetPosition());
	Effects::SkyFX->SetPlanetPosW(control.get_earthPosW());
	Effects::SkyFX->SetDirLights(mDirLights);
	Effects::SkyFX->SetCameraPos(cameraPos);
	Effects::SkyFX->SetLightPos(sunPos);
	Effects::SkyFX->SetInvWaveLength(invWaveLength);
	Effects::SkyFX->SetCameraHeight(height);
	Effects::SkyFX->SetCameraHeight2(height*height);
	Effects::SkyFX->SetOuterRadius(outerRadius);
	Effects::SkyFX->SetOuterRadius2(outerRadius * outerRadius);
	Effects::SkyFX->SetInnerRadius(innerRadius);
	Effects::SkyFX->SetInnerRadius2(innerRadius * innerRadius);
	Effects::SkyFX->SetKrESun(Kr * ESun);
	Effects::SkyFX->SetKmESun(Km * ESun);
	Effects::SkyFX->SetKr4PI(Kr * 4.0f * MathHelper::Pi);
	Effects::SkyFX->SetKm4PI(Km * 4.0f * MathHelper::Pi);
	Effects::SkyFX->SetScale( scale );
	Effects::SkyFX->SetScaleOverScaleDepth( scale / 0.25f );
	Effects::SkyFX->SetG(-0.990f);
	Effects::SkyFX->SetG2((-0.990f)*(-0.990f));
	
	// SPACE EFFECTS
	Effects::SpaceFX->SetEyePosW(control.get_Camera().GetPosition());
	Effects::SpaceFX->SetPlanetPosW(control.get_earthPosW());
	Effects::SpaceFX->SetCameraPos(cameraPos);
	Effects::SpaceFX->SetLightPos(sunPos);
	Effects::SpaceFX->SetInvWaveLength(invWaveLength);
	Effects::SpaceFX->SetCameraHeight(height);
	Effects::SpaceFX->SetCameraHeight2(height*height);
	Effects::SpaceFX->SetOuterRadius(outerRadius);
	Effects::SpaceFX->SetOuterRadius2(outerRadius * outerRadius);
	Effects::SpaceFX->SetInnerRadius(innerRadius);
	Effects::SpaceFX->SetInnerRadius2(innerRadius * innerRadius);
	Effects::SpaceFX->SetKrESun(Kr * ESun);
	Effects::SpaceFX->SetKmESun(Km * ESun);
	Effects::SpaceFX->SetKr4PI(Kr * 4.0f * MathHelper::Pi);
	Effects::SpaceFX->SetKm4PI(Km * 4.0f * MathHelper::Pi);
	Effects::SpaceFX->SetScale( scale );
	Effects::SpaceFX->SetScaleOverScaleDepth( scale / 0.25f );
	Effects::SpaceFX->SetG(-0.990f);
	Effects::SpaceFX->SetG2((-0.990f)*(-0.990f));
	
	// DISPLACEMENT MAPPING EFFECTS
	Effects::DisplacementMapFX->SetEyePosW(control.get_Camera().GetPosition());
	Effects::DisplacementMapFX->SetPlanetPosW(control.get_earthPosW());
	Effects::DisplacementMapFX->SetCameraPos(cameraPos);
	Effects::DisplacementMapFX->SetLightPos(sunPos);
	Effects::DisplacementMapFX->SetInvWaveLength(invWaveLength);
	Effects::DisplacementMapFX->SetCameraHeight(height);
	Effects::DisplacementMapFX->SetCameraHeight2(height*height);
	Effects::DisplacementMapFX->SetOuterRadius(outerRadius);
	Effects::DisplacementMapFX->SetOuterRadius2(outerRadius * outerRadius);
	Effects::DisplacementMapFX->SetInnerRadius(innerRadius);
	Effects::DisplacementMapFX->SetInnerRadius2(innerRadius * innerRadius);
	Effects::DisplacementMapFX->SetKrESun(Kr * ESun);
	Effects::DisplacementMapFX->SetKmESun(Km * ESun);
	Effects::DisplacementMapFX->SetKr4PI(Kr * 4.0f * MathHelper::Pi);
	Effects::DisplacementMapFX->SetKm4PI(Km * 4.0f * MathHelper::Pi);
	Effects::DisplacementMapFX->SetScale( scale );
	Effects::DisplacementMapFX->SetScaleOverScaleDepth( scale / 0.25f );
	Effects::DisplacementMapFX->SetG(-0.990f);
	Effects::DisplacementMapFX->SetG2((-0.990f)*(-0.990f));
	Effects::DisplacementMapFX->SetDirLights(mDirLights);
	
	// CLOUDS EFFECTS
	Effects::CloudsFX->SetEyePosW(control.get_Camera().GetPosition());
	Effects::CloudsFX->SetPlanetPosW(control.get_earthPosW());
	Effects::CloudsFX->SetCameraPos(cameraPos);
	Effects::CloudsFX->SetLightPos(sunPos);
	Effects::CloudsFX->SetInvWaveLength(invWaveLength);
	Effects::CloudsFX->SetCameraHeight(height);
	Effects::CloudsFX->SetCameraHeight2(height*height);
	Effects::CloudsFX->SetOuterRadius(outerRadius);
	Effects::CloudsFX->SetOuterRadius2(outerRadius * outerRadius);
	Effects::CloudsFX->SetInnerRadius(innerRadius);
	Effects::CloudsFX->SetInnerRadius2(innerRadius * innerRadius);
	Effects::CloudsFX->SetKrESun(Kr * ESun);
	Effects::CloudsFX->SetKmESun(Km * ESun);
	Effects::CloudsFX->SetKr4PI(Kr * 4.0f * MathHelper::Pi);
	Effects::CloudsFX->SetKm4PI(Km * 4.0f * MathHelper::Pi);
	Effects::CloudsFX->SetScale( scale );
	Effects::CloudsFX->SetScaleOverScaleDepth( scale / 0.25f );
	Effects::CloudsFX->SetG(-0.990f);
	Effects::CloudsFX->SetG2((-0.990f)*(-0.990f));
	Effects::CloudsFX->SetDirLights(mDirLights);
	 
	md3dImmediateContext->IASetInputLayout(InputLayouts::PosNormalTexTan);
	md3dImmediateContext->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dImmediateContext->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	
	if( GetAsyncKeyState('1') & 0x8000 )
		md3dImmediateContext->RSSetState(RenderStates::WireframeRS);
		
	// DRAW THE EARTH
	md3dImmediateContext->OMSetDepthStencilState(RenderStates::PlanetDSS,255);
	if ( height > outerRadius )
		activeTech = Effects::DisplacementMapFX->PlanetFromSpaceTech;
	else
		activeTech = Effects::DisplacementMapFX->PlanetFromAtmoTech;
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	Effects::DisplacementMapFX->SetHeightScale(29.029f);
	Effects::DisplacementMapFX->SetMaxTessDistance(0.10f);
	Effects::DisplacementMapFX->SetMinTessDistance(3000.0f);
	Effects::DisplacementMapFX->SetMinTessFactor(1.0f);
	Effects::DisplacementMapFX->SetMaxTessFactor(100.0f);
	activeTech->GetDesc( &techDesc );
    for(UINT p = 0; p < techDesc.Passes; ++p)
    {
		switch(mRenderOptions)
		{
		case RenderOptionsBasic:
			Effects::BasicFX->SetWorld(world);
			Effects::BasicFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::BasicFX->SetWorldViewProj(worldViewProj);
			Effects::BasicFX->SetTexTransform(reinterpret_cast<CXMMATRIX>(mTexTransform));
			Effects::BasicFX->SetMaterial(mEarthMat);
			Effects::BasicFX->SetDiffuseMap(mEarthDiffuseMapSRV);
			break;
		case RenderOptionsNormalMap:
			Effects::NormalMapFX->SetWorld(world);
			Effects::NormalMapFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::NormalMapFX->SetWorldViewProj(worldViewProj);
			Effects::NormalMapFX->SetTexTransform(reinterpret_cast<CXMMATRIX>(mTexTransform));
			Effects::NormalMapFX->SetMaterial(mEarthMat);
			Effects::NormalMapFX->SetDiffuseMap(mEarthDiffuseMapSRV);
			Effects::NormalMapFX->SetNormalMap(mEarthNormalTexSRV);
			break;
		case RenderOptionsDisplacementMap:
			Effects::DisplacementMapFX->SetWorld(world);
			Effects::DisplacementMapFX->SetWorldInvTranspose(worldInvTranspose);
			Effects::DisplacementMapFX->SetViewProj(viewProj);
			Effects::DisplacementMapFX->SetWorldViewProj(worldViewProj);
			Effects::DisplacementMapFX->SetTexTransform(reinterpret_cast<CXMMATRIX>(mTexTransform));
			Effects::DisplacementMapFX->SetMaterial(mEarthMat);
			Effects::DisplacementMapFX->SetDiffuseMap(mEarthDiffuseMapSRV);
			Effects::DisplacementMapFX->SetNormalMap(mEarthNormalTexSRV);
			break;
		}
		
		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mEarthIndexCount, mEarthIndexOffset, mEarthVertexOffset);
    }
	
	// DRAW THE CLOUDS
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	md3dImmediateContext->OMSetBlendState(RenderStates::TransparentBS, blendFactor, 0xffffffff);
	if ( height > outerRadius ) {
		md3dImmediateContext->OMSetDepthStencilState(RenderStates::DontWriteDepthDSS,0);
		activeTech = Effects::CloudsFX->CloudsFromSpaceTech;
	}
	else {
		md3dImmediateContext->OMSetDepthStencilState(RenderStates::CloudDSS,0);
		md3dImmediateContext->RSSetState(RenderStates::ReverseWindingRS);
		activeTech = Effects::CloudsFX->CloudsFromAtmoTech;
	}

	Effects::CloudsFX->SetHeightScale(control.get_skyAltitude()/2.0f);
	Effects::CloudsFX->SetMaxTessDistance(1.0f);
	Effects::CloudsFX->SetMinTessDistance(1000.0f);
	Effects::CloudsFX->SetMinTessFactor(1.0f);
	Effects::CloudsFX->SetMaxTessFactor(1.0f);

	Effects::CloudsFX->SetWorld(world);
	Effects::CloudsFX->SetWorldInvTranspose(worldInvTranspose);
	Effects::CloudsFX->SetViewProj(viewProj);
	Effects::CloudsFX->SetWorldViewProj(worldViewProj);
	Effects::CloudsFX->SetTexTransform(reinterpret_cast<CXMMATRIX>(mTexTransform));
	Effects::CloudsFX->SetMaterial(mEarthMat);
	Effects::CloudsFX->SetDiffuseMap(mCloudsDiffuseMapSRV);
	Effects::CloudsFX->SetNormalMap(mCloudsNormalTexSRV);

	activeTech->GetDesc( &techDesc );

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {		
		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mCloudsIndexCount, mCloudsIndexOffset, mCloudsVertexOffset);
    }
	// restore default states
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);

	
	// FX sets tessellation stages, but it does not disable them.  So do that here
	// to turn off tessellation.
	md3dImmediateContext->HSSetShader(0, 0, 0);
	md3dImmediateContext->DSSetShader(0, 0, 0);	

	// DRAW THE SKY
	if ( height > outerRadius ) {
		activeTech = Effects::SkyFX->SkyFromSpaceTech;
	}
	else {
		activeTech = Effects::SkyFX->SkyFromAtmoTech;
	}
	md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	activeTech->GetDesc( &techDesc );
	
	md3dImmediateContext->RSSetState(RenderStates::CullFrontRS);

    for(UINT p = 0; p < techDesc.Passes; ++p)
    {		
		Effects::SkyFX->SetWorld(world);
		Effects::SkyFX->SetWorldInvTranspose(worldInvTranspose);
		Effects::SkyFX->SetWorldViewProj(worldViewProj);
		Effects::SkyFX->SetMaterial(mEarthMat);
				
		activeTech->GetPassByIndex(p)->Apply(0, md3dImmediateContext);
		md3dImmediateContext->DrawIndexed(mSkyIndexCount, mSkyIndexOffset, mSkyVertexOffset);
    }
	// restore default states
	md3dImmediateContext->RSSetState(0);
	md3dImmediateContext->OMSetDepthStencilState(0, 0);
#endif

	HR(mSwapChain->Present(0, 0));
}

void Renderer::BuildGeometryBuffers()
{
#if USE_QUADTREE
	earth = Ellipsoid(control.get_earthRadius()/2.0f, control.get_earthRadius()/2.0f, control.get_earthRadius()/2.0f);
	earth.generateMeshes( 7 );
	
	std::vector<Object::Vertex> earthVerts = earth.getVertices();
	std::vector<Vertex::PosNormalTexTan> vertices( earthVerts.size() );
	
	UINT k = 0;
	for(size_t i = 0; i < earthVerts.size(); ++i, ++k)
	{
		vertices[k].Pos			= earthVerts[i].Position;
		vertices[k].Normal		= earthVerts[i].Normal;
		vertices[k].Tex			= earthVerts[i].TexC;
		vertices[k].TangentU	= earthVerts[i].TangentU;
	}
	
	
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan) * vertices.size();
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

#else
	GeometryGenerator::MeshData earthMesh;
	GeometryGenerator::MeshData skyMesh;
	GeometryGenerator::MeshData cloudMesh;

	GeometryGenerator geoGen;
	
	geoGen.CreateGeosphere(control.get_earthRadius() + control.get_skyAltitude(), 7.0f, skyMesh);
	geoGen.CreateGeosphere(control.get_earthRadius(), 4.0f, cloudMesh);
	geoGen.CreateGeosphere(control.get_earthRadius(),5.0f,earthMesh);
	
	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	mEarthVertexOffset      = 0;
	// Cache the starting index for each object in the concatenated index buffer.
	mEarthIndexOffset      = 0;

	// Cache the index count of each object.
	mEarthIndexCount      = earthMesh.Indices.size();
	mSkyIndexCount		  = skyMesh.Indices.size();
	mCloudsIndexCount	  = cloudMesh.Indices.size();
	
	UINT totalVertexCount = earthMesh.Vertices.size() + skyMesh.Vertices.size() + cloudMesh.Indices.size();

	UINT totalIndexCount = mEarthIndexCount + mSkyIndexCount + mCloudsIndexCount;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	std::vector<Vertex::PosNormalTexTan> vertices(totalVertexCount);

	UINT k = 0;
	for(size_t i = 0; i < earthMesh.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos			= earthMesh.Vertices[i].Position;
		vertices[k].Normal		= earthMesh.Vertices[i].Normal;
		vertices[k].Tex			= earthMesh.Vertices[i].TexC;
		vertices[k].TangentU	= earthMesh.Vertices[i].TangentU;
	}
	
	mSkyVertexOffset = k;

	for(size_t i = 0; i < skyMesh.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos			= skyMesh.Vertices[i].Position;
		vertices[k].Normal		= skyMesh.Vertices[i].Normal;
		vertices[k].Tex			= skyMesh.Vertices[i].TexC;
		vertices[k].TangentU	= skyMesh.Vertices[i].TangentU;
	}

	mCloudsVertexOffset = k;

	for(size_t i = 0; i < cloudMesh.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos			= cloudMesh.Vertices[i].Position;
		vertices[k].Normal		= cloudMesh.Vertices[i].Normal;
		vertices[k].Tex			= cloudMesh.Vertices[i].TexC;
		vertices[k].TangentU	= cloudMesh.Vertices[i].TangentU;
	}

    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex::PosNormalTexTan) * totalVertexCount;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = &vertices[0];
    HR(md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB));

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	std::vector<UINT> indices;
	indices.insert(indices.end(), earthMesh.Indices.begin(), earthMesh.Indices.end());
	mSkyIndexOffset = indices.size();
	indices.insert(indices.end(), skyMesh.Indices.begin(), skyMesh.Indices.end());
	mCloudsIndexOffset = indices.size();
	indices.insert(indices.end(), cloudMesh.Indices.begin(), cloudMesh.Indices.end());

	D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = &indices[0];
    HR(md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB));
#endif
}
 
