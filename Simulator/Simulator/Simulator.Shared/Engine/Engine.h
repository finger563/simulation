#pragma once

#include "Base\Subsystem.h"

#include "Base\Objects\GameObject.h"
#include "Base\Objects\Ellipsoid.h"

#include "Physics\Physics.h"
#include "Input\Input.h"

namespace Engine
{
	class Engine
	{
	private:
	public:
		Engine();
		~Engine();

		bool StartUp();
		void Update();
		bool ShutDown();
	};
}