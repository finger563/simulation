#pragma once

#include "Base\Subsystem.h"

namespace Physics
{
	interface IPhysics : public Base::ISubsystem
	{
		virtual void SetSimulationType() = 0;

		virtual void StepObjects() = 0;
	};

	class Physics : public IPhysics
	{
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void OnSuspending();
		virtual void OnResuming();

		virtual void SetSimulationType();

		virtual void StepObjects();
	};
}