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
		SELECTION
	};

	enum ValueTypes
	{
		FLOAT,
		INT,
		BOOL
	};

	struct InputValue
	{
		InputTypes inputType;
		ValueTypes valueType;
		union
		{
			float fval;
			int ival;
			bool bval;
		};

		void Initialize(InputTypes iType, ValueTypes vType);
	};
	
	interface class IInput : public Base::ISubsystem
	{
	};

	ref class Input : public IInput
	{
	private:
		std::vector<InputValue> inputs;
		void ClearInputs();
	internal:
		void SetInputTypes(
			std::vector<InputTypes>& iTypes,
			std::vector<ValueTypes>& vTypes
			);
		std::vector<InputValue>& GetInputs();
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		void KeyDown(CoreWindow^ Window, KeyEventArgs^ Args);
		void KeyUp(CoreWindow^ Window, KeyEventArgs^ Args);
		void PointerPressed(CoreWindow^ Window, PointerEventArgs^ Args);
		void PointerWheelChanged(CoreWindow^ Window, PointerEventArgs^ Args);
	};
}