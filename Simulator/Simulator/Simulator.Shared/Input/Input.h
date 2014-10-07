#pragma once

#include "Base\Base.h"

using namespace Windows::UI::Core;

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
	private:
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void SetInputTypes();

		virtual void GetInputUpdates();

		void KeyDown(CoreWindow^ Window, KeyEventArgs^ Args);
		void KeyUp(CoreWindow^ Window, KeyEventArgs^ Args);
		void PointerPressed(CoreWindow^ Window, PointerEventArgs^ Args);
		void PointerWheelChanged(CoreWindow^ Window, PointerEventArgs^ Args);
	};
}