#pragma once

#include "Base\Base.h"

using namespace Windows::UI::Core;

namespace Input
{
	enum ValueOperations { ADD, SUBTRACT, MULTIPLY, DIVIDE };
	enum ValueTypes { FLOAT, INT, BOOL };

	struct InputValue
	{
		float Magnitude;
		ValueTypes Type;
		union
		{
			float fval;
			int ival;
			bool bval;
		};
		InputValue() { fval = 0; }
		InputValue(ValueTypes t, float mag);
	};

	ref class Input		// needs to be ref (makes it WinRT class) so that it's functions can be used for callbacks 
	{
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		void KeyDown(CoreWindow^ Window, KeyEventArgs^ Args);
		void KeyUp(CoreWindow^ Window, KeyEventArgs^ Args);
		void PointerPressed(CoreWindow^ Window, PointerEventArgs^ Args);
		void PointerWheelChanged(CoreWindow^ Window, PointerEventArgs^ Args);
	internal:			// used in this project only
		bool AddInput(
			Platform::String^ name,
			ValueTypes t,
			float magnitude,
			Windows::System::VirtualKey initialKey
			);
		InputValue& GetInput(Platform::String^ name);
	private:
		std::map<Platform::String^, InputValue> nameToValue;
		std::map<Windows::System::VirtualKey, InputValue*> keyToValuePtr;
		void ClearInputs();
	};
}