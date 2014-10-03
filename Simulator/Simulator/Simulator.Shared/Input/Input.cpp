#include "pch.h"
#include "Input.h"

using namespace Windows::System;
using namespace Windows::UI::Popups;

namespace Input
{

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
		return;
	}

	void Input::SetInputTypes()
	{

	}

	void Input::GetInputUpdates()
	{

	}

	void Input::KeyDown(CoreWindow^ Window, KeyEventArgs^ Args)
	{
		if (Args->VirtualKey == VirtualKey::A ||
			Args->VirtualKey == VirtualKey::S ||
			Args->VirtualKey == VirtualKey::W ||
			Args->VirtualKey == VirtualKey::D)
		{
			// do something..
			MessageDialog Dialog("You pressed a key!", "Notice!");
			Dialog.ShowAsync();
		}
	}

	void Input::KeyUp(CoreWindow^ Window, KeyEventArgs^ Args)
	{
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