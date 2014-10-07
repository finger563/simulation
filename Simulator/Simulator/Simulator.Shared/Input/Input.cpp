#include "pch.h"
#include "Input.h"

using namespace Windows::System;
using namespace Windows::UI::Popups;

namespace Input
{

	void InputValue::Initialize(InputTypes iType, ValueTypes vType)
	{
		valueType = vType;
		inputType = iType;
	}

	bool Input::Initialize()
	{
		ClearInputs();
		return true;
	}

	bool Input::UnInitialize()
	{
		return true;
	}

	void Input::Update()
	{
		ClearInputs();
	}

	void Input::ClearInputs()
	{
		for (unsigned int i = 0; i < inputs.size(); i++)
		{
			inputs[i].fval = 0.0f;
		}
	}

	void Input::SetInputTypes(
		std::vector<InputTypes>& iTypes,
		std::vector<ValueTypes>& vTypes
		)
	{
		assert( iTypes.size() == vTypes.size() );
		for (unsigned int i = 0; i < iTypes.size(); i++)
		{
			inputs.push_back(InputValue());
			inputs.back().Initialize(iTypes[i], vTypes[i]);
		}
	}

	std::vector<InputValue>& Input::GetInputs()
	{
		return inputs;
	}

	void Input::KeyDown(CoreWindow^ Window, KeyEventArgs^ Args)
	{
		switch (Args->VirtualKey)
		{
		case (VirtualKey::A) :
			inputs[1].fval -= 0.5f;
			break;
		case (VirtualKey::D) :
			inputs[1].fval += 0.5f;
			break;
		case (VirtualKey::W) :
			inputs[0].fval += 0.5f;
			break;
		case (VirtualKey::S) :
			inputs[0].fval -= 0.5f;
			break;
		default:
			break;
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