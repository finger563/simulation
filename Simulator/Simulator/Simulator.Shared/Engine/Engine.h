#pragma once

#include "Base\Subsystem.h"

#include "Base\Objects\GameObject.h"
#include "Base\Objects\Ellipsoid.h"

#include "Physics\Physics.h"
#include "Input\Input.h"
#include "Renderer\Renderer.h"

namespace Engine
{
	class Engine : public Base::ISubsystem
	{
	private:
		Physics::Physics	mPhysics;
		Input::Input		mInput;
		Renderer::Renderer	mRenderer;
	public:
		bool Initialize();
		void Update();
		bool UnInitialize();
	};
}