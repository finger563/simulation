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
	
	interface class IInput : public Base::ISubsystem
	{
		virtual void SetInputTypes() = 0;

		virtual void GetInputUpdates() = 0;
	};

	ref class Input : public IInput
	{
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void SetInputTypes();

		virtual void GetInputUpdates();
	};
}