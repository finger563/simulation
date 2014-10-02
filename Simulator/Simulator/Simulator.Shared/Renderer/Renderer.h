#pragma once

#include "Base\Subsystem.h"
#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;

namespace Renderer
{
	template<typename T>
	interface IRenderer : public Base::ISubsystem
	{
		virtual void SetCamera(
			Vector<3,T> Position, 
			Vector<4,T> Orientation, 
			T NearPlane, 
			T FarPlane, 
			T FOV, 
			T AspectRatio
			) = 0;

		virtual void SetObjects(std::vector<T>& Objects) = 0;

		virtual void Render() = 0;
	};

	template<typename T>
	class Renderer : public IRenderer<T>
	{
	public:
		bool Initialize();
		void Update();
		bool UnInitialize();

		void SetCamera(
			Vector<3, T> Position,
			Vector<4, T> Orientation,
			T NearPlane,
			T FarPlane,
			T FOV,
			T AspectRatio
			);

		void SetObjects(std::vector<T>& Objects);

		void Render();
	};
}