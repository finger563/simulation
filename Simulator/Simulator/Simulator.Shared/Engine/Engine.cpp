#include "pch.h"
#include "Engine.h"

namespace Engine
{

	Engine::Engine()
	{
		mRenderer = ref new Renderer::Renderer();
		mInput = ref new Input::Input();
		mPhysics = ref new Physics::Physics();
	}

	bool Engine::Initialize()
	{
		mRenderer->Initialize();
		mInput->Initialize();
		mPhysics->Initialize();
		return true;
	}

	bool Engine::UnInitialize()
	{
		return true;
	}

	void Engine::Update()
	{
		mInput->Update();
		mPhysics->Update();
		mRenderer->Update();
		mRenderer->Render();
		return;
	}

	Input::Input^ Engine::GetInput()
	{
		return mInput;
	}
}