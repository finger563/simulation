#include "pch.h"
#include "PGM.h"

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
	PGM::PGM(const std::shared_ptr<DeviceResources>& devResources) :
		deviceResources(devResources)
	{
	}

	bool PGM::Initialize()
	{
		// THIS IS JUST FOR TESTING
		primaryRadius = 1.0f;
		sphereWorldPos = Base::Math::VectorInit({ 0.0f, 0.0f, 0.0f });
		numGridPointsX = 16;
		numGridPointsY = 16;

		// We need to configure the window-dependent resources and members:
		// Obtain a pointer to the window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		// Need to init the view camera
		ViewCamera.Set(
			Base::Math::VectorInit({ 0.0, 0.0, -5.0, 0.0 }),			// position
			Base::Math::VectorInit({ 0.0, 0.0, 1.0, 0.0 }),				// view
			Base::Math::VectorInit({ 0.0, 1.0, 0.0, 0.0 }),				// up
			45.0f,														// FOVY
			(float)(Window->Bounds.Width / Window->Bounds.Height),	// aspect ratio
			1.0f,														// near plane
			1000.0f														// far plane
			);

		// Need to init the sampling camera
		SamplingCamera.Set(
			Base::Math::VectorInit({ 0.0, 0.0, -5.0, 0.0 }),			// position
			Base::Math::VectorInit({ 0.0, 0.0, 1.0, 0.0 }),				// view
			Base::Math::VectorInit({ 0.0, 1.0, 0.0, 0.0 }),				// up
			45.0f,														// FOVY
			(float)(Window->Bounds.Width / Window->Bounds.Height),	// aspect ratio
			1.0f,														// near plane
			1000.0f														// far plane
			);

		pgmShader = std::make_unique<Shader>(deviceResources, "PGMVertexShader.cso","", "PGMGeometryShader.cso");
		pgmShader->SetInputDescriptor(pgmIED,1);
		pgmShader->Initialize();
		// END JUST FOR TESTING
		
		// create the vertex buffer for stream out between PGM projection and rasterization stages
		D3D11_BUFFER_DESC vertexBD = { 0 };
		vertexBD.Usage = D3D11_USAGE_DEFAULT;
		vertexBD.ByteWidth = numGridPointsX * numGridPointsY;
		vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
		vertexBD.CPUAccessFlags = 0;
		vertexBD.MiscFlags = 0;
		
		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&vertexBD, nullptr, &streamOutVertexBuffer)
			);

		return true;
	}

	void PGM::CreateWindowSizeDependentResources()
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

		ViewCamera.OrientMatrix = XMLoadFloat4x4(&orientation);

		SamplingCamera.OrientMatrix = XMLoadFloat4x4(&orientation);

		MakeGridPoints();
	}

	void PGM::Update()
	{
		// set our new render target object as the active render target
		auto context = deviceResources->GetD3DDeviceContext();
		ID3D11RenderTargetView *const targets[] = { deviceResources->GetBackBufferRenderTargetView() };
		context->OMSetRenderTargets(1, targets, deviceResources->GetDepthStencilView());

		// Reset the viewport to target the whole screen.
		auto viewport = deviceResources->GetScreenViewport();
		context->RSSetViewports(1, &viewport);
		
		// clear the back buffer
		context->ClearRenderTargetView(deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);

		// clear the depth buffer
		context->ClearDepthStencilView(deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// set up render target buffers for Deferred Rendering:
		//  * Pass 1:
		//		input:
		//		* vertex/index buffers
		//		output:
		//		* positions
		//		* normals
		//	* Pass 2:
		//		input:
		//		* position buffer
		//		* normal buffer
		//		* texture atlas
		//		* height atlas
		//		output:
		//		* rendered scene

		pgmShader->Apply();

		// update view camera
		ViewCamera.UpdateMatrices();
		// update sampling camera
		SamplingCamera.UpdateMatrices();
		// compute ray-sphere intersection point & sphere intersection normal
		//   gamma1 & gamma 2
		//		* gamma1 = asin((d / r) sin w) - w : first intersection angle from nadir
		//		* gamma2 = -asin((d / r) sin w) - w + pi : second intersection angle from nadir

		// set up constant buffers for planet data to GPU for processing in shaders

		// update grid points (per the new sampling viewport)
		//	* calc new extents to edges of sphere for frusta
		//	* calc new distance to planet
		// build grid points into vertex buffer and index buffer
		// send grid points:
		// set the vertex buffer

		// PGM Stage:
		//	* send PGM info (camera info, shape info) in constant buffer
		//	* set grid points vertex buffer / index buffer created for sampling camera to IA stage
		//	* set the stream out vertex buffer to stream out stage

		UINT stride = sizeof(PGM::Vertex);
		UINT offset = 0;
		// set the vertex buffer (grid points)
		context->IASetVertexBuffers(0, 1, gridvertexbuffer.GetAddressOf(), &stride, &offset);
		// set the index buffer (grid points)
		context->IASetIndexBuffer(gridindexbuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		// set the stream-out buffer for output from Geometry Shader
		context->SOSetTargets(1,streamOutVertexBuffer.GetAddressOf(),&offset);

		// set the primitive topology
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// update the constant buffers with relevant info for PGM (camera & surface info)
		PGM_Pass0_CBuffer pgmCbuffer;
		pgmCbuffer.CameraPosition = SamplingCamera.Position;
		pgmCbuffer.ViewVector = SamplingCamera.View;
		context->UpdateSubresource(pgmShader->constantbuffer.Get(), 0, 0, &pgmCbuffer, 0, 0);

		// invoke the shader code for raycasting PGM
		context->DrawIndexed(numIndices, 0, 0);

		// Rasterization Stage:
		//	* set stream-out stage target to null
		//	* send main camera info & transform from sampling to main camera
		//	* set stream out vertex buffer (from PGM stage) to IA stage

		// clear stream out stage target from previous stage
		ComPtr<ID3D11Buffer> bufferArray = { 0 };
		context->SOSetTargets(1, bufferArray.GetAddressOf(), &offset);

		stride = sizeof(Base::Vertex);
		offset = 0;
		// set the vertex buffer (grid points)
		context->IASetVertexBuffers(0, 1, gridvertexbuffer.GetAddressOf(), &stride, &offset);

		// set the primitive topology
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// update the constant buffers with relevant info for PGM (camera & surface info)
		PGM_Pass1_CBuffer rasterizationCbuffer;
		rasterizationCbuffer.CameraPosition = SamplingCamera.Position;
		rasterizationCbuffer.ViewVector = SamplingCamera.View;
		context->UpdateSubresource(pgmShader->constantbuffer.Get(), 0, 0, &rasterizationCbuffer, 0, 0);

		// invoke the shader code for raycasting PGM
		context->DrawIndexed(numIndices, 0, 0);

	}

	void PGM::MakeGridPoints()
	{
		numIndices = numGridPointsX * numGridPointsY;
		std::vector<PGM::Vertex> OurVertices;
		std::vector<UINT> OurIndices;
		// need to set up verts & inds based on grid
		
		// create the vertex buffer
		D3D11_BUFFER_DESC vertexBD = { 0 };
		vertexBD.ByteWidth = sizeof(PGM::Vertex) * numIndices;
		vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSRD = { OurVertices.data(), 0, 0 };

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&vertexBD, &vertexSRD, &gridvertexbuffer)
			);

		// create the index buffer
		// buffer description
		D3D11_BUFFER_DESC indexBD = { 0 };
		indexBD.ByteWidth = sizeof(UINT) * (int)OurIndices.size();    // indices are stored in short values
		indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// subresource data
		D3D11_SUBRESOURCE_DATA indexSRD = { OurIndices.data(), 0, 0 };

		ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(&indexBD, &indexSRD, &gridindexbuffer)
			);
	}

	bool PGM::UnInitialize()
	{
		return true;
	}

	void PGM::OnSuspending()
	{

	}

	void PGM::OnResuming()
	{

	}
}