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
			Vector<3, float>({ 0.0, 0.0, -5.0 }),						// position
			Vector<3, float>({ 0.0, 0.0, 1.0 }),						// view
			Vector<3, float>({ 0.0, 1.0, 0.0 }),						// up
			45.0f,														// FOVY
			(float)(windowProperties.Width / windowProperties.Height),	// aspect ratio
			1.0f,														// near plane
			1000.0f														// far plane
			);

		mInput->Initialize();
		std::vector<Input::InputTypes> iTypes = 
		{ 
			Input::InputTypes::DIRECTIONAL, Input::InputTypes::DIRECTIONAL, // WASD
			Input::InputTypes::ROTATIONAL, Input::InputTypes::ROTATIONAL // up/down/left/right
		};
		std::vector<Input::ValueTypes> vTypes = 
		{ 
			Input::ValueTypes::FLOAT, Input::ValueTypes::FLOAT,
			Input::ValueTypes::FLOAT, Input::ValueTypes::FLOAT
		};
		mInput->SetInputTypes(iTypes, vTypes);
		mPhysics->Initialize();
		return true;
	}

	bool Engine::UnInitialize()
	{
		return true;
	}

	void Engine::Update()
	{
		std::vector<Input::InputValue>& inputs = mInput->GetInputs();
		mRenderer->Pitch(inputs[2].fval);
		mRenderer->Yaw(inputs[3].fval);
		mRenderer->Walk(inputs[0].fval);
		mRenderer->Strafe(inputs[1].fval);
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