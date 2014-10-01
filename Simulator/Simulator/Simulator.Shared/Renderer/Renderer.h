#pragma once

#include "Base\Subsystem.h"
#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"

namespace Renderer
{
	interface Renderer : public Base::ISubsystem
	{
		template<typename T>
		virtual void SetCamera(
			Vector<3,T> Position, 
			Vector<4,T> Orientation, 
			T NearPlane, 
			T FarPlane, 
			T FOV, 
			T AspectRatio
			) = 0;

		template<typename T>
		virtual void SetObjects(std::vector<T>& Objects) = 0;

		virtual void Render() = 0;
	};
}