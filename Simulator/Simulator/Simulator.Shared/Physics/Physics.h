#pragma once

#include "Base\Subsystem.h"

namespace Physics
{
	class Physics : public Base::Subsystem
	{
	private:
	public:
		Physics();
		~Physics();

		bool StartUp();
		void Update();
		bool ShutDown();
	};
}