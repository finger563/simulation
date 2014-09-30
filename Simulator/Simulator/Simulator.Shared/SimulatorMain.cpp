#include "pch.h"
#include "SimulatorMain.h"
#include "Renderer\Common\DirectXHelper.h"

#include "Base\Math\Matrix.h"
#include "Base\Objects\GameObject.h"
#include "Base\Data Structures\QuadTree.h"
#include "Base\Objects\BoundingObjects.h"
#include "Base\Objects\Ellipsoid.h"

using namespace Simulator;

using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

using namespace Base::Math;

Mat3x3d mymat = Mat3x3d({ 1.0, 0.0, 0.0,
0.0, 1.0, 0.0,
0.0, 0.0, 1.0 });

Vec3d myvec3 = Vec3d({ 1.0, 2.0 });
Vec4d myvec4 = Vec4d({ 1.0, 2.0 });
Vector<5, double> newVec = Vector<5, double>(myvec4, 10.0);

// Loads and initializes application assets when the application is loaded.
SimulatorMain::SimulatorMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

SimulatorMain::~SimulatorMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void SimulatorMain::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// Updates the application state once per frame.
void SimulatorMain::Update() 
{
	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
	});
}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool SimulatorMain::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	m_sceneRenderer->Render();

	m_fpsTextRenderer->m_text = L"";
	for (int i = 0; i < 9; i++)
	{
		m_fpsTextRenderer->m_text += L" " + std::to_wstring(mymat.vals[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		m_fpsTextRenderer->m_text += L" " + std::to_wstring(newVec[i]);
	}
	Vec3d geo({
		M_PI_2,
		M_PI_4,
		0
	});
	Vec2d tex = Base::Objects::Ellipsoid<double>::geodeticToTexture(geo);
	for (int i = 0; i < 2; i++)
	{
		m_fpsTextRenderer->m_text += L" " + std::to_wstring(tex[i]);
	}
	m_fpsTextRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void SimulatorMain::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void SimulatorMain::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
