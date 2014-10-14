#pragma once

#include "Base\Base.h"
#include "Physics\Physics.h"
#include "Input\Input.h"
#include "Renderer\Renderer.h"

namespace Engine
{
	class Engine : public Base::ISubsystem
	{
		struct WindowProperties
		{
			double Width;
			double Height;
		};
	private:
		double SimulationTime;
		std::vector<Base::Objects::GameObject<float>> gameObjects;
		WindowProperties windowProperties;
	public:
		Engine();

		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void OnSuspending();
		virtual void OnResuming();

		void SetWindowProperties(double width, double height);

		Input::Input^ GetInput();

		std::unique_ptr<Physics::Physics>	mPhysics;
		std::unique_ptr<Renderer::Renderer>	mRenderer;

		Input::Input^						mInput;
	};
}