#pragma once

#include "Base\Base.h"
#include "Physics\Physics.h"
#include "Input\Input.h"
#include "Renderer\Renderer.h"

namespace Engine
{
	ref class Engine sealed : public Base::ISubsystem
	{
		struct WindowProperties
		{
			double Width;
			double Height;
		};
	internal: // only used by code in this project
		void SetWindowProperties(double width, double height);
	private:
		double SimulationTime;
		std::vector<Base::Objects::GameObject<float>> gameObjects;
		WindowProperties windowProperties;
	public:
		Engine();

		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		Input::Input^ GetInput();

		property Physics::Physics^		mPhysics;
		property Input::Input^			mInput;
		property Renderer::Renderer^	mRenderer;
	};
}