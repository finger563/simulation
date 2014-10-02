#pragma once

#include "Base\Subsystem.h"

#include "Base\Objects\GameObject.h"
#include "Base\Objects\Ellipsoid.h"

#include "Physics\Physics.h"
#include "Input\Input.h"
#include "Renderer\Renderer.h"

namespace Engine
{
	class Engine
	{
	public:
		Physics::Physics<double>	mPhysics;
		Input::Input<double>		mInput;
		Renderer::Renderer<double>	mRenderer;
	};
}