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
		mRenderer->SetCamera(
			Vector<3, double>({ 5.0, 0.0, -5.0 }),				// position
			Vector<4, double>({ 0.0, 0.0, 0.0, 0.0 }),			// orientation
			45.0,												// FOVY
			windowProperties.Width / windowProperties.Height,	// aspect ratio
			1.0,												// near plane
			1000.0												// far plane
			);

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

	void Engine::SetWindowProperties(double width, double height)
	{
		windowProperties.Height = height;
		windowProperties.Width = width;
	}

	Input::Input^ Engine::GetInput()
	{
		return mInput;
	}
}