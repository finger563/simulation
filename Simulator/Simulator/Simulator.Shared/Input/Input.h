#pragma once

#include "Base\Subsystem.h"

namespace Input
{
	enum InputTypes
	{
		DIRECTIONAL,
		ROTATIONAL,
		BOOLEAN,
		SEQUENTIAL,
		FLOATING,
		MODIFIER,
		TREE
	};
	
	interface IInput : public Base::ISubsystem
	{
		virtual void SetInputTypes() = 0;

		virtual void GetInputUpdates() = 0;
	};

	class Input : public IInput
	{
	public:
		bool Initialize();
		void Update();
		bool UnInitialize();

		void SetInputTypes();

		void GetInputUpdates();
	};
}