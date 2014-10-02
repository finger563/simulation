#pragma once

#include "Base\Subsystem.h"

namespace Physics
{
	template<typename T>
	interface IPhysics : public Base::ISubsystem
	{
		virtual void SetSimulationType(T SimulationType) = 0;

		virtual void StepObjects(std::vector<T>& Objects, T DeltaT) = 0;
	};

	template<typename T>
	class Physics : public IPhysics<T>
	{
	public:
		bool Initialize();
		void Update();
		bool UnInitialize();

		void SetSimulationType(T SimulationType);

		void StepObjects(std::vector<T>& Objects, T DeltaT);
	};
}