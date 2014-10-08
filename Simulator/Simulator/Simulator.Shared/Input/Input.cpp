#include "pch.h"
#include "Input.h"

using namespace Windows::System;
using namespace Windows::UI::Popups;

namespace Input
{

	InputValue::InputValue(ValueTypes t, float mag)
	{
		Type = t;
		Magnitude = mag;
		fval = 0;
	}

	bool Input::Initialize()
	{
		return true;
	}

	bool Input::UnInitialize()
	{
		return true;
	}

	void Input::Update()
	{
	}

	bool Input::AddInput(
		Platform::String^ name,
		ValueTypes t,
		float magnitude,
		VirtualKey initialKey
		)
	{
		if (keyToValuePtr.find(initialKey) != keyToValuePtr.end())
			return false;		// another input already uses this key
		nameToValue[name] = InputValue(t, magnitude);
		keyToValuePtr[initialKey] = &nameToValue[name];
		return true;
	}

	InputValue& Input::GetInput(Platform::String^ name)
	{
		return nameToValue[name];
	}

	void Input::KeyDown(CoreWindow^ Window, KeyEventArgs^ Args)
	{
		if (keyToValuePtr.find(Args->VirtualKey) == keyToValuePtr.end())
			return;			// this Key is not currently listed as an input
		InputValue* val = keyToValuePtr[Args->VirtualKey];
		val->fval = val->Magnitude;
	}

	void Input::KeyUp(CoreWindow^ Window, KeyEventArgs^ Args)
	{
		if (keyToValuePtr.find(Args->VirtualKey) == keyToValuePtr.end())
			return;			// this Key is not currently listed as an input
		InputValue* val = keyToValuePtr[Args->VirtualKey];
		val->fval = 0;
	}

	void Input::PointerPressed(CoreWindow^ Window, PointerEventArgs^ Args)
	{
		MessageDialog Dialog("Thank you for clicking me!", "Notice!");
		Dialog.ShowAsync();
	}

	void Input::PointerWheelChanged(CoreWindow^ Window, PointerEventArgs^ Args)
	{
		int Wheel = Args->CurrentPoint->Properties->MouseWheelDelta; // increments of 120; + is up, - is down
	}
}