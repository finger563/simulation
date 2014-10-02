#pragma once
#include "pch.h"

namespace Base
{
	interface class ISubsystem
	{
		virtual bool Initialize() = 0;
		virtual void Update() = 0;
		virtual bool UnInitialize() = 0;
	};
}