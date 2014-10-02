#pragma once

#include "Base\Subsystem.h"

namespace Input
{
	template<typename T>
	interface IInput : public Base::ISubsystem
	{
		virtual void SetInputTypes(T InputTypes) = 0;

		virtual void GetInputUpdates(std::vector<T>& Inputs) = 0;
	};

	template<typename T>
	class Input : public IInput<T>
	{
	public:
		bool Initialize();
		void Update();
		bool UnInitialize();

		void SetInputTypes(T InputTypes);

		void GetInputUpdates(std::vector<T>& Inputs);
	};
}