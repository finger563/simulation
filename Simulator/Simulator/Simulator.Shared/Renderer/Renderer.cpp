#include "pch.h"
#include "Renderer.h"

namespace Renderer
{

	template<typename T>
	bool Renderer<T>::Initialize()
	{
		return true;
	}

	template<typename T>
	bool Renderer<T>::UnInitialize()
	{
		return true;
	}

	template<typename T>
	void Renderer<T>::Update()
	{
		return;
	}

	template<typename T>
	void Renderer<T>::Render()
	{
		return;
	}

	template<typename T>
	void Renderer<T>::SetCamera(
		Vector<3, T> Position,
		Vector<4, T> Orientation,
		T NearPlane,
		T FarPlane,
		T FOV,
		T AspectRatio
		)
	{
		return;
	}

	template<typename T>
	void Renderer<T>::SetObjects(std::vector<T>& Objects)
	{
		return;
	}

}