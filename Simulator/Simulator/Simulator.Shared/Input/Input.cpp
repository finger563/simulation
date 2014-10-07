#include "pch.h"
#include "Input.h"

using namespace Windows::System;
using namespace Windows::UI::Popups;

namespace Input
{
	InputValue::~InputValue()
	{
		if (numDimensions > 0)
		{
			switch (valueType)
			{
			case ValueTypes::FLOAT:
				delete[] fvals;
				break;
			case ValueTypes::INT:
				delete[] ivals;
				break;
			case ValueTypes::BOOL:
				delete[] bvals;
				break;
			case ValueTypes::INPUT:
				delete[] children;
				break;
			}
			delete[] fvals;
		}
	}

	void InputValue::Initialize(InputTypes iType, ValueTypes vType, int dimensions)
	{
		numDimensions = dimensions;
		valueType = vType;
		inputType = iType;
		switch (vType)
		{
		case ValueTypes::FLOAT:
			fvals = new float[numDimensions];
			break;
		case ValueTypes::INT:
			ivals = new int[numDimensions];
			break;
		case ValueTypes::BOOL:
			bvals = new bool[numDimensions];
			break;
		case ValueTypes::INPUT:
			children = new InputValue[numDimensions];
			break;
		}
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
		return;
	}

	void Input::SetInputTypes(
		std::vector<InputTypes>& iTypes,
		std::vector<ValueTypes>& vTypes,
		std::vector<int>& dims
		)
	{
		assert( iTypes.size() == vTypes.size() && vTypes.size() == dims.size() );
		for (unsigned int i = 0; i < iTypes.size(); i++)
		{
			inputs.push_back(InputValue());
			inputs.back().Initialize(iTypes[i], vTypes[i], dims[i]);
		}
	}

	std::vector<InputValue>& Input::GetInputUpdates()
	{
		return inputs;
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