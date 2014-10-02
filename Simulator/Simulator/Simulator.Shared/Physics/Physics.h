#pragma once

#include "Base\Subsystem.h"

namespace Physics
{
	interface Physics : public Base::ISubsystem
	{
		template<typename T>
		virtual void SetSimulationType(T SimulationType) = 0;

		template<typename T1, typename T2>
		virtual void StepObjects(std::vector<T1>& Objects, T2 deltaT) = 0;
	};
}