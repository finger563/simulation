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
		bool Initialize();
		void Update();
		bool UnInitialize();

		void SetSimulationType();

		void StepObjects();
	};
}