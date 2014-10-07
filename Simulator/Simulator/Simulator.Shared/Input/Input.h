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

	enum ValueTypes
	{
		FLOAT,
		INT,
		BOOL,
		INPUT
	};

	class InputValue
	{
	private:
		InputTypes inputType;
		ValueTypes valueType;
		int numDimensions;
	public:
		union
		{
			float* fvals;
			int* ivals;
			bool* bvals;
			InputValue* children;
		};
		InputValue() { numDimensions = 0; fvals = nullptr; }
		~InputValue();
		void Initialize(InputTypes iType, ValueTypes vType, int dimensions);
	};
	
	interface class IInput : public Base::ISubsystem
	{
	};

	ref class Input : public IInput
	{
	private:
		std::vector<InputValue> inputs;
	internal:
		void SetInputTypes(
			std::vector<InputTypes>& iTypes,
			std::vector<ValueTypes>& vTypes,
			std::vector<int>& dims
			);
		std::vector<InputValue>& GetInputUpdates();
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