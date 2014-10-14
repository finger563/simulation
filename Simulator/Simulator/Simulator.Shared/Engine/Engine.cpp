#include "pch.h"
#include "Engine.h"

namespace Engine
{

	Engine::Engine()
	{
		mRenderer = std::unique_ptr<Renderer::Renderer>(new Renderer::Renderer());
		mPhysics = std::unique_ptr<Physics::Physics>(new Physics::Physics());

		mInput = ref new Input::Input();
	}

	bool Engine::Initialize()
	{
		SimulationTime = 0;
		mRenderer->Initialize();
		mRenderer->SetCamera(
			Vector<3, float>({ 0.0, 0.0, -5.0 }),						// position
			Vector<3, float>({ 0.0, 0.0, 1.0 }),						// view
			Vector<3, float>({ 0.0, 1.0, 0.0 }),						// up
			45.0f,														// FOVY
			(float)(mWindow->Bounds.Width / mWindow->Bounds.Height),	// aspect ratio
			1.0f,														// near plane
			1000.0f														// far plane
			);

		gameObjects.push_back(Base::Objects::GameObject<float>());
		Renderer::BaseObjects::InitCubeMesh(&gameObjects.back().mesh);
		mRenderer->SetObjectsInScene(&gameObjects);

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

	void Engine::OnSuspending()
	{
		mRenderer->OnSuspending();
		mPhysics->OnSuspending();
		mInput->OnSuspending();
	}

	void Engine::OnResuming()
	{
		mRenderer->OnResuming();
		mPhysics->OnResuming();
		mInput->OnResuming();
	}

	void Engine::Update()
	{
		SimulationTime += 0.05;

		mInput->Update();

		float walk = 0;
		walk += mInput->GetInput("WalkForward").fval;
		walk -= mInput->GetInput("WalkBackward").fval;
		float strafe = 0;
		strafe += mInput->GetInput("StrafeRight").fval;
		strafe -= mInput->GetInput("StrafeLeft").fval;
		float pitch = 0;
		pitch += mInput->GetInput("PitchUp").fval;
		pitch -= mInput->GetInput("PitchDown").fval;
		float yaw = 0;
		yaw += mInput->GetInput("YawRight").fval;
		yaw -= mInput->GetInput("YawLeft").fval;

		mRenderer->GetCameraPtr()->Pitch(pitch);
		mRenderer->GetCameraPtr()->Yaw(yaw);
		mRenderer->GetCameraPtr()->Walk(walk);
		mRenderer->GetCameraPtr()->Strafe(strafe);

		gameObjects[0].position.vals[0] = (float)cos(SimulationTime) * 3.0f;
		gameObjects[0].position.vals[2] = (float)sin(SimulationTime) * 3.0f;

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