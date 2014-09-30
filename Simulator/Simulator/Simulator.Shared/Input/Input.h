#pragma once

namespace Input
{
	class Input : public Base::Subsystem
	{
	private:
	public:
		Input();
		~Input();

		bool StartUp();
		void Update();
		bool ShutDown();
	};
}