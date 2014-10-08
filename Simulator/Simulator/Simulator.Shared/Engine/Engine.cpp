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

		// Maybe have these in an initialization file?
		mInput->AddInput(
			"WalkForward",
			Input::ValueTypes::FLOAT,
			0.5f,
			Windows::System::VirtualKey::W
			);
		mInput->AddInput(
			"WalkBackward",
			Input::ValueTypes::FLOAT,
			0.5f,
			Windows::System::VirtualKey::S
			);
		mInput->AddInput(
			"StrafeRight",
			Input::ValueTypes::FLOAT,
			0.5f,
			Windows::System::VirtualKey::D
			);
		mInput->AddInput(
			"StrafeLeft",
			Input::ValueTypes::FLOAT,
			0.5f,
			Windows::System::VirtualKey::A
			);

		mInput->AddInput(
			"YawRight",
			Input::ValueTypes::FLOAT,
			0.05f,
			Windows::System::VirtualKey::Right
			);
		mInput->AddInput(
			"YawLeft",
			Input::ValueTypes::FLOAT,
			0.05f,
			Windows::System::VirtualKey::Left
			);

		mInput->AddInput(
			"PitchUp",
			Input::ValueTypes::FLOAT,
			0.05f,
			Windows::System::VirtualKey::Up
			);
		mInput->AddInput(
			"PitchDown",
			Input::ValueTypes::FLOAT,
			0.05f,
			Windows::System::VirtualKey::Down
			);

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

		float walk = 0;
		float strafe = 0;
		float pitch = 0;
		float yaw = 0;
		walk += mInput->GetInput("WalkForward").fval;
		walk -= mInput->GetInput("WalkBackward").fval;
		strafe += mInput->GetInput("StrafeRight").fval;
		strafe -= mInput->GetInput("StrafeLeft").fval;
		pitch += mInput->GetInput("PitchUp").fval;
		pitch -= mInput->GetInput("PitchDown").fval;
		yaw += mInput->GetInput("YawRight").fval;
		yaw -= mInput->GetInput("YawLeft").fval;
		mRenderer->Pitch(pitch);
		mRenderer->Yaw(yaw);
		mRenderer->Walk(walk);
		mRenderer->Strafe(strafe);

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