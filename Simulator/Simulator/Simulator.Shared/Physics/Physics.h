#pragma once

#include "Base\Subsystem.h"

namespace Physics
{
	interface class IPhysics : public Base::ISubsystem
	{
		virtual void SetSimulationType() = 0;

		virtual void StepObjects() = 0;
	};

	ref class Physics : public IPhysics
	{
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void SetSimulationType();

		virtual void StepObjects();
	};
}