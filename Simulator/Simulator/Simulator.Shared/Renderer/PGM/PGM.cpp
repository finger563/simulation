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

		ViewCamera.Set(
			Base::Math::VectorInit({ 0.0, 0.0, -5.0, 0.0 }),			// position
			Base::Math::VectorInit({ 0.0, 0.0, 1.0, 0.0 }),				// view
			Base::Math::VectorInit({ 0.0, 1.0, 0.0, 0.0 }),				// up
			45.0f,														// FOVY
			(float)(Window->Bounds.Width / Window->Bounds.Height),	// aspect ratio
			1.0f,														// near plane
			1000.0f														// far plane
			);

		pgmShader = std::make_unique<Shader>(deviceResources, "PGMVertexShader.cso","PGMPixelShader.cso");
		pgmShader->Initialize();
		// END JUST FOR TESTING

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
	}

	void PGM::Update()
	{
		// update view camera
		ViewCamera.UpdateMatrices();
		// update grid points (per the new viewport)
		// compute ray-sphere intersection point & sphere intersection normal
		//   gamma1 & gamma 2
		//		* gamma1 = asin((d / r) sin w) - w : first intersection angle from nadir
		//		* gamma2 = -asin((d / r) sin w) - w + pi : second intersection angle from nadir
		// update sampling camera

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