#pragma once

#include "Base\Subsystem.h"

namespace Renderer
{
	class Renderer : public Base::Subsystem
	{
	private:
	public:
		Renderer();
		~Renderer();

		bool StartUp();
		void Update();
		bool ShutDown();
	};
}