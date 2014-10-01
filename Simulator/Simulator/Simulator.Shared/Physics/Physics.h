#pragma once

#include "Base\Subsystem.h"

namespace Physics
{
	interface Physics : public Base::ISubsystem
	{
		template<typename T>
		virtual void SetSimulationType(T SimulationType) = 0;

		template<typename T>
		virtual void StepObjects(std::vector<T>& Objects) = 0;
	};
}