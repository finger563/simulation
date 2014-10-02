#pragma once

#include "Base\Subsystem.h"

#include "Base\Objects\GameObject.h"
#include "Base\Objects\Ellipsoid.h"

#include "Physics\Physics.h"
#include "Input\Input.h"
#include "Renderer\Renderer.h"

namespace Engine
{
	ref class Engine : public Base::ISubsystem
	{
	private:
		Physics::Physics^	mPhysics;
		Input::Input^		mInput;
		Renderer::Renderer^	mRenderer;
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();
	};
}