#include "pch.h"
#include "Renderer.h"

using namespace concurrency;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

namespace Renderer
{
	Renderer::Renderer(const std::shared_ptr<DeviceResources>& devResources) :
		deviceResources(devResources),
		pgm(devResources),
		updatePGM(true)
	{
	}

	void *Renderer::operator new(size_t size)
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
		pgm.Initialize();

		directionalLights.push_back(
			DirectionalLight( 
				XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f),	// position
				XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f),	// diffuse
				XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f),	// ambient
				XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f)		// specular
				)
			);

		objects = nullptr;

		// We need to configure the window-dependent resources and members:
		// Obtain a pointer to the window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		deviceResources->SetWindow(Window);

		InitStates();
		
		shader = std::make_unique<Shader>(deviceResources);

		shader->Initialize(sizeof(DefaultCBuffer));

		return true;
	}

	void Renderer::CreateWindowSizeDependentResources()
	{
		Size outputSize = deviceResources->GetOutputSize();
		float aspectRatio = outputSize.Width / outputSize.Height;
		float fovAngleY = 70.0f * XM_PI / 180.0f;

		// This is a simple example of change that can be made when the app is in
		// portrait or snapped view.
		if (aspectRatio < 1.0f)
		{
			fovAngleY *= 2.0f;
		}

		// Note that the OrientationTransform3D matrix is post-multiplied here
		// in order to correctly orient the scene to match the display orientation.
		// This post-multiplication step is required for any draw calls that are
		// made to the swap chain render target. For draw calls to other targets,
		// this transform should not be applied.

		XMFLOAT4X4 orientation = deviceResources->GetOrientationTransform3D();

		camera.OrientMatrix = XMLoadFloat4x4(&orientation);
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

	void Renderer::SetUpdatePGM(Platform::Boolean update)
	{
		updatePGM = update;
	}

	Platform::Boolean Renderer::GetUpdatePGM()
	{
		return updatePGM;
	}

	void Renderer::SetRasterizerState(Platform::String^ state)
	{
		auto context = deviceResources->GetD3DDeviceContext();		
		if (state->Equals("default"))
		{
			context->RSSetState(defaultrasterizerstate.Get());
		}
		else if (state->Equals("wireframe"))
		{
			context->RSSetState(wireframerasterizerstate.Get());
		}
	}

	bool Renderer::UnInitialize()
	{
		return true;
	}

	void Renderer::Update()
	{
		auto context = deviceResources->GetD3DDeviceContext();

		// set our new render target object as the active render target
		ID3D11RenderTargetView *const targets[] = { deviceResources->GetBackBufferRenderTargetView() };
		context->OMSetRenderTargets(1, targets, deviceResources->GetDepthStencilView());

		// Reset the viewport to target the whole screen.
		auto viewport = deviceResources->GetScreenViewport();
		context->RSSetViewports(1, &viewport);

		// clear the back buffer to a deep blue
		context->ClearRenderTargetView(deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);

		// clear the depth buffer
		context->ClearDepthStencilView(deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		pgm.SetViewCamera(camera);
		if (updatePGM)
		{
			pgm.SetSamplingCamera(camera);
			pgm.Update();
		}
	}

	void Renderer::Render()
	{
		pgm.Render();

		shader->Apply();

		auto context = deviceResources->GetD3DDeviceContext();

		// set the vertex buffer
		UINT stride = sizeof(Base::Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), &stride, &offset);
		// set the index buffer
		context->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// set the primitive topology
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		XMMATRIX matTranslate = XMMatrixTranslationFromVector((*objects)[0].position);
		XMMATRIX matScale = XMMatrixScalingFromVector((*objects)[0].scale);
		XMMATRIX matRotateX = XMMatrixRotationX(XMVectorGetByIndex((*objects)[0].orientation,0));
		XMMATRIX matRotateY = XMMatrixRotationY(XMVectorGetByIndex((*objects)[0].orientation,1));
		XMMATRIX matRotateZ = XMMatrixRotationZ(XMVectorGetByIndex((*objects)[0].orientation,2));
		XMMATRIX matRotation = matRotateX * matRotateY * matRotateZ;

		XMMATRIX matWorld = matRotation * matScale * matTranslate;

		camera.UpdateMatrices();
		XMMATRIX matFinal = matWorld * camera.ViewMatrix * camera.ProjMatrix * camera.OrientMatrix;

		DefaultCBuffer cbuffer;
		cbuffer.matWVP = matFinal;

		// Lighting related
		cbuffer.matRotation = matRotation;	// need to get from object
		cbuffer.DiffuseVector = directionalLights[0].Position;
		cbuffer.DiffuseColor = directionalLights[0].Diffuse;
		cbuffer.AmbientColor = directionalLights[0].Ambient;

		// set the new values for the constant buffer
		context->UpdateSubresource(shader->constantbuffer.Get(), 0, 0, &cbuffer, 0, 0);
		
		context->DrawIndexed(numIndices, 0, 0);

		shader->Disable();

		return;
	}

	void Renderer::SetCamera( Vector position, Vector view, Vector up,
		float FoVY, float AspectRatio, float NearPlane, float FarPlane )
	{
		camera.Set(
			position,
			view,
			up,
			FoVY,
			AspectRatio,
			NearPlane,
			FarPlane
			);
	}
	
	void Renderer::SetObjectsInScene(std::vector<Base::Objects::GameObject>* _objects)
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

	void Renderer::OnSuspending()
	{
		deviceResources->Trim();
	}

	void Renderer::OnResuming()
	{

	}

#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
	void Renderer::OnWindowSizeChanged(CoreWindow^ Sender, WindowSizeChangedEventArgs^ Args)
	{
		deviceResources->SetLogicalSize(Size(Sender->Bounds.Width, Sender->Bounds.Height));
		CreateWindowSizeDependentResources();
	}
#endif

	// DisplayInformation event handlers.

	void Renderer::OnDisplayContentsInvalidated(DisplayInformation^ Sender, Object^ Args)
	{
		deviceResources->ValidateDevice();
	}

#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
	void Renderer::OnDpiChanged(DisplayInformation^ Sender, Object^ Args)
	{
		deviceResources->SetDpi(Sender->LogicalDpi);
		CreateWindowSizeDependentResources();
	}

	void Renderer::OnOrientationChanged(DisplayInformation^ Sender, Object^ Args)
	{
		deviceResources->SetCurrentOrientation(Sender->CurrentOrientation);
		CreateWindowSizeDependentResources();
	}
#endif

	// Notifies renderers that device resources need to be released.
	void Renderer::ReleaseDeviceDependentResources()
	{
		vertexbuffer.Reset();
		indexbuffer.Reset();
		defaultrasterizerstate.Reset();
		wireframerasterizerstate.Reset();

		shader->Reset();
	}

	// Notifies renderers that device resources may now be recreated.
	void Renderer::CreateDeviceDependentResources()
	{
		shader->Initialize(sizeof(DefaultCBuffer));
	}
}