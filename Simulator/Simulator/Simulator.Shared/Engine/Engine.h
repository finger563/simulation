#pragma once

#include "Base\Base.h"
#include "Physics\Physics.h"
#include "Input\Input.h"
#include "Renderer\Renderer.h"

namespace Engine
{
	class Engine : public Base::ISubsystem, public Renderer::IDeviceNotify
	{
	private:
		double SimulationTime;
		std::vector<Base::Objects::GameObject<float>> gameObjects;
	public:
		Engine();

		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void OnSuspending();
		virtual void OnResuming();

		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();
		
		Input::Input^ GetInput();

		std::unique_ptr<Physics::Physics>		mPhysics;
		std::unique_ptr<Renderer::Renderer>		mRenderer;
		std::unique_ptr<Renderer::TextRenderer>	mTextRenderer;

		std::shared_ptr<Renderer::DeviceResources> deviceResources;

		Input::Input^						mInput;

		// Cached reference to the Window.
		Platform::Agile<Windows::UI::Core::CoreWindow> mWindow;
	};
}