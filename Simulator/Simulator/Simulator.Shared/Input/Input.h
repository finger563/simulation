#pragma once

#include "Base\Subsystem.h"

namespace Input
{
	interface Input : public Base::ISubsystem
	{
		template<typename T>
		virtual void SetInputTypes(T InputTypes) = 0;

		template<typename T>
		virtual void GetInputUpdates(std::vector<T>& Inputs) = 0;
	};
}