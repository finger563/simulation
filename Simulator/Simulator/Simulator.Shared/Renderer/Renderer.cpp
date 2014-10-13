#include "pch.h"
#include "Renderer.h"

namespace Renderer
{
	Renderer::Renderer()
	{
	}

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

		objects = nullptr;

		// Constructor here makes all the D3D11 and D2D initialization
		// Note: it only does the window-independent init
		deviceResources = std::make_shared<DeviceResources>();
		// We need to configure the window-dependent resources and members:
		// Obtain a pointer to the window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		deviceResources->SetWindow(Window);
		// Register to be notified if the Device is lost or recreated
		deviceResources->RegisterDeviceNotify(this);

		InitStates();
		
		shader = std::make_unique<Shader>(deviceResources);

		shader->Initialize();
		shader->Apply();

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

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateRasterizerState(&rd, &defaultrasterizerstate)
			);

		// set the changed values for wireframe mode
		rd.FillMode = D3D11_FILL_WIREFRAME;
		rd.AntialiasedLineEnable = TRUE;

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateRasterizerState(&rd, &wireframerasterizerstate)
			);
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
		auto context = deviceResources->GetD3DDeviceContext();

		// set our new render target object as the active render target
		ID3D11RenderTargetView *const targets[] = { deviceResources->GetBackBufferRenderTargetView() };
		context->OMSetRenderTargets(1, targets, deviceResources->GetDepthStencilView());

		// Reset the viewport to target the whole screen.
		auto viewport = deviceResources->GetScreenViewport();
		context->RSSetViewports(1, &viewport);

		// clear the back buffer to a deep blue
		context->ClearRenderTargetView(deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
		
		// clear the depth buffer
		context->ClearDepthStencilView(deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// set the vertex buffer
		UINT stride = sizeof(Base::Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), &stride, &offset);
		// set the index buffer
		context->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// set the primitive topology
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
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
		context->UpdateSubresource(shader->constantbuffer.Get(), 0, 0, &cbuffer, 0, 0);

		// Set the rasterizer state here if we want to
		//devcon->RSSetState(wireframerasterizerstate.Get());
		// or
		context->RSSetState(defaultrasterizerstate.Get());

		// draw 3 vertices, starting from vertex 0
		context->DrawIndexed(numIndices, 0, 0);

		deviceResources->GetSwapChain()->Present(1, 0);	// swap the back buffer and the front buffer
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
		// update the objects pointer that we use in the renderer
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

		// update the number of indices to be rendered by the renderer
		numIndices = (int)OurIndices.size();
		
		// create the vertex buffer
		D3D11_BUFFER_DESC vertexBD = { 0 };
		vertexBD.ByteWidth = sizeof(Base::Vertex) * (int)OurVertices.size();
		vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSRD = { OurVertices.data() , 0, 0 };

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&vertexBD, &vertexSRD, &vertexbuffer)
			);

		// create the index buffer
		// buffer description
		D3D11_BUFFER_DESC indexBD = { 0 };
		indexBD.ByteWidth = sizeof(UINT) * (int)OurIndices.size();    // indices are stored in short values
		indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// subresource data
		D3D11_SUBRESOURCE_DATA indexSRD = { OurIndices.data() , 0, 0 };

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&indexBD, &indexSRD, &indexbuffer)
			);
	}


	// Notifies renderers that device resources need to be released.
	void Renderer::OnDeviceLost()
	{
	}

	// Notifies renderers that device resources may now be recreated.
	void Renderer::OnDeviceRestored()
	{
	}
}