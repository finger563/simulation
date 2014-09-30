#pragma once

namespace Base
{
	class Subsystem
	{
	protected:
	public:
		Subsystem();
		~Subsystem();

		virtual bool StartUp() = 0;
		virtual void Update() = 0;
		virtual bool ShutDown() = 0;
	};
}