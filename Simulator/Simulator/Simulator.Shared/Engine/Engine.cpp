#include "pch.h"
#include "Engine.h"

namespace Engine
{

	bool Engine::Initialize()
	{
		mRenderer.Initialize();
		return true;
	}

	bool Engine::UnInitialize()
	{
		return true;
	}

	void Engine::Update()
	{
		mRenderer.Update();
		mRenderer.Render();
		return;
	}
}