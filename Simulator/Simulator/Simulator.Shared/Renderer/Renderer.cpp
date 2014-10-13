#include "pch.h"
#include "Renderer.h"

namespace Renderer
{
	void* Renderer::operator new(size_t size)
	{
		// XMVECTOR && XMMATRIX must be 16 byte aligned
		void* storage = _aligned_malloc(size, 16);
		if (nullptr == storage)
			throw "ALLOCATION FAIL : NO FREE MEMORY";
		return storage;
	}

	void Renderer::operator delete(void* ptr)
	{
		_aligned_free(ptr);
	}

	bool Renderer::Initialize()
	{
		directionalLights.push_back(
			DirectionalLight( 
				XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f),	// position
				XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f),	// diffuse
				XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),	// ambient
				XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f)		// specular
				)
			);

		// EVERYTHING UNDER HERE IS REQUIRED TO BE PART OF THIS FUNCTION FOR NOW

		objects = nullptr;

		// Define temporary pointers to a device and a device context
		ComPtr<ID3D11Device> dev11;
		ComPtr<ID3D11DeviceContext> devcon11;

		// Create the device and device context objects
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&dev11,
			nullptr,
			&devcon11);

		// Convert the pointers from the DirectX 11 versions to the DirectX 11.1 versions
		dev11.As(&dev);
		devcon11.As(&devcon);

		// First, convert our ID3D11Device2 into an IDXGIDevice2
		ComPtr<IDXGIDevice2> dxgiDevice;
		dev.As(&dxgiDevice);

		// Second, use the IDXGIDevice2 interface to get access to the adapter
		ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetAdapter(&dxgiAdapter);

		// Third, use the IDXGIAdapter interface to get access to the factory
		ComPtr<IDXGIFactory2> dxgiFactory;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

		// set up the swap chain description
		DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how the swap chain should be used
		scd.BufferCount = 2;                                  // a front buffer and a back buffer
		scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // the most common swap chain format
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;    // the recommended flip mode
		scd.SampleDesc.Count = 1;                             // disable anti-aliasing
 
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();    // get the window pointer

		dxgiFactory->CreateSwapChainForCoreWindow(
			dev.Get(),                                  // address of the device
			reinterpret_cast<IUnknown*>(Window),        // address of the window
			&scd,                                       // address of the swap chain description
			nullptr,                                    // advanced
			&swapchain);                                // address of the new swap chain pointer

		// TO RENDER TO THE BACK BUFFER
		// get a pointer directly to the back buffer
		ComPtr<ID3D11Texture2D> backbuffer;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);

		// create a render target pointing to the back buffer
		dev->CreateRenderTargetView(backbuffer.Get(), nullptr, &rendertarget);

		// set the viewport
		D3D11_VIEWPORT viewport = { 0 };

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Window->Bounds.Width;
		viewport.Height = Window->Bounds.Height;
		viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
		viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

		devcon->RSSetViewports(1, &viewport);

		// enabling Depth Buffering
		D3D11_TEXTURE2D_DESC texd = { 0 };

		texd.Width = (UINT)(Window->Bounds.Width);
		texd.Height = (UINT)(Window->Bounds.Height);
		texd.ArraySize = 1;
		texd.MipLevels = 1;
		//texd.SampleDesc.Count = 1;
		texd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		ComPtr<ID3D11Texture2D> zbuffertexture;
		dev->CreateTexture2D(&texd, nullptr, &zbuffertexture);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));

		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		dev->CreateDepthStencilView(zbuffertexture.Get(), &dsvd, &zbuffer);

		InitStates();

		// EVERYTHING ABOVE HERE IS REQUIRED TO BE PART OF THIS FUNCTION FOR NOW
		
		shader.SetHandles(dev, devcon);
		shader.Initialize();
		shader.Apply();

		return true;
	}

	void Renderer::CreateWindowSizeDependentResources()
	{

	}

	void Renderer::InitStates()
	{
		D3D11_RASTERIZER_DESC rd;
		rd.FillMode = D3D11_FILL_SOLID;
		rd.CullMode = D3D11_CULL_BACK;
		rd.FrontCounterClockwise = FALSE;
		rd.DepthClipEnable = TRUE;
		rd.ScissorEnable = FALSE;
		rd.AntialiasedLineEnable = FALSE;
		rd.MultisampleEnable = FALSE;
		rd.DepthBias = 0;
		rd.DepthBiasClamp = 0.0f;
		rd.SlopeScaledDepthBias = 0.0f;

		dev->CreateRasterizerState(&rd, &defaultrasterizerstate);

		// set the changed values for wireframe mode
		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.AntialiasedLineEnable = TRUE;

		dev->CreateRasterizerState(&rd, &wireframerasterizerstate);
	}

	bool Renderer::UnInitialize()
	{
		return true;
	}

	void Renderer::Update()
	{

	}

	void Renderer::Render()
	{
		// set our new render target object as the active render target
		devcon->OMSetRenderTargets(1, rendertarget.GetAddressOf(), zbuffer.Get());
		// can use this to set multiple render targets (number, list)

		// clear the depth buffer
		devcon->ClearDepthStencilView(zbuffer.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		// clear the back buffer to a deep blue
		float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
		devcon->ClearRenderTargetView(rendertarget.Get(), color);

		// set the vertex buffer
		UINT stride = sizeof(Base::Vertex);
		UINT offset = 0;
		devcon->IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), &stride, &offset);
		// set the index buffer
		devcon->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// set the primitive topology
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		XMMATRIX matTranslate = XMMatrixTranslation((*objects)[0].position[0], (*objects)[0].position[1], (*objects)[0].position[2]);
		XMMATRIX matScale = XMMatrixScaling((*objects)[0].scale[0], (*objects)[0].scale[1], (*objects)[0].scale[2]);
		XMMATRIX matRotateX = XMMatrixRotationX((*objects)[0].orientation[0]);
		XMMATRIX matRotateY = XMMatrixRotationY((*objects)[0].orientation[1]);
		XMMATRIX matRotateZ = XMMatrixRotationZ((*objects)[0].orientation[2]);
		XMMATRIX matRotation = matRotateX * matRotateY * matRotateZ;

		XMMATRIX matWorld = matRotation * matScale * matTranslate;

		camera.UpdateMatrices();
		XMMATRIX matFinal = matWorld * camera.ViewMatrix * camera.ProjMatrix;

		CBuffer cbuffer;
		cbuffer.matWVP = matFinal;

		// Lighting related
		cbuffer.matRotation = matRotation;	// need to get from object
		cbuffer.DiffuseVector = directionalLights[0].Position;
		cbuffer.DiffuseColor = directionalLights[0].Diffuse;
		cbuffer.AmbientColor = directionalLights[0].Ambient;

		// set the new values for the constant buffer
		devcon->UpdateSubresource(shader.constantbuffer.Get(), 0, 0, &cbuffer, 0, 0);

		// Set the rasterizer state here if we want to
		//devcon->RSSetState(wireframerasterizerstate.Get());
		// or
		devcon->RSSetState(defaultrasterizerstate.Get());

		// draw 3 vertices, starting from vertex 0
		devcon->DrawIndexed(numIndices, 0, 0);

		swapchain->Present(1, 0);	// swap the back buffer and the front buffer
		return;
	}

	void Renderer::SetCamera(
		Vector<3, float> position,
		Vector<3, float> view,
		Vector<3, float> up,
		float FoVY,
		float AspectRatio,
		float NearPlane,
		float FarPlane
		)
	{
		camera.Set(
			XMVectorSet(position[0], position[1], position[2], 0),
			XMVectorSet(view[0], view[1], view[2], 0),
			XMVectorSet(up[0], up[1], up[2], 0),
			FoVY,
			AspectRatio,
			NearPlane,
			FarPlane
			);
	}
	
	void Renderer::SetObjectsInScene(std::vector<Base::Objects::GameObject<float>>* _objects)
	{
		objects = _objects;
		numIndices = 0;
		std::vector<Base::Vertex> OurVertices;
		std::vector<UINT> OurIndices;
		for (unsigned int i = 0; i < _objects->size(); i++)
		{
			for (unsigned int j = 0; j < (*_objects)[i].mesh->vertices.size(); j++)
			{
				OurVertices.push_back((*_objects)[i].mesh->vertices[j]);
			}
			for (unsigned int j = 0; j < (*_objects)[i].mesh->indices.size(); j++)
			{
				OurIndices.push_back((*_objects)[i].mesh->indices[j]);
			}
		}

		numIndices = OurIndices.size();
		
		// create the vertex buffer
		D3D11_BUFFER_DESC vertexBD = { 0 };
		vertexBD.ByteWidth = sizeof(Base::Vertex) * OurVertices.size();
		vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSRD = { OurVertices.data() , 0, 0 };

		dev->CreateBuffer(&vertexBD, &vertexSRD, &vertexbuffer);

		// create the index buffer
		// buffer description
		D3D11_BUFFER_DESC indexBD = { 0 };
		indexBD.ByteWidth = sizeof(UINT) * OurIndices.size();    // indices are stored in short values
		indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// subresource data
		D3D11_SUBRESOURCE_DATA indexSRD = { OurIndices.data() , 0, 0 };

		dev->CreateBuffer(&indexBD, &indexSRD, &indexbuffer);
	}
}