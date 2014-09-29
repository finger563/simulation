#pragma once

#include "Base\Time\StepTimer.h"
#include "Renderer\Common\DeviceResources.h"
#include "Renderer\Content\Sample3DSceneRenderer.h"
#include "Renderer\Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace Simulator
{
	class SimulatorMain : public DX::IDeviceNotify
	{
	public:
		SimulatorMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~SimulatorMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		// Rendering loop timer.
		Base::Time::StepTimer m_timer;
	};
}