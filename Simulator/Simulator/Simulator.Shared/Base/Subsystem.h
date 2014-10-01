#pragma once
#include "pch.h"

namespace Base
{
	interface ISubsystem
	{
		virtual bool StartUp() = 0;
		virtual void Update() = 0;
		virtual bool ShutDown() = 0;
	};
}