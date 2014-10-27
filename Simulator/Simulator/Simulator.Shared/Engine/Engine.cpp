#include "pch.h"
#include "Engine.h"

namespace Engine
{

	Renderer::roam myroam;
	Engine::Engine()
	{
		// need to initialize input here; it needs to be used before initialize() is called
		mInput = ref new Input::Input();
	}

	bool Engine::Initialize()
	{
		SimulationTime = 0;

		// Constructor here makes all the D3D11 and D2D initialization
		// Note: it only does the window-independent init
		deviceResources = std::make_shared<Renderer::DeviceResources>();
		// Register to be notified if the Device is lost or recreated
		deviceResources->RegisterDeviceNotify(this);

		mRenderer = std::unique_ptr<Renderer::Renderer>(new Renderer::Renderer(deviceResources));

		mRenderer->Initialize();
		mRenderer->SetCamera(
			Base::Math::VectorInit({ 0.0, 0.0, -5.0, 0.0 }),			// position
			Base::Math::VectorInit({ 0.0, 0.0, 1.0, 0.0 }),				// view
			Base::Math::VectorInit({ 0.0, 1.0, 0.0, 0.0 }),				// up
			45.0f,														// FOVY
			(float)(mWindow->Bounds.Width / mWindow->Bounds.Height),	// aspect ratio
			1.0f,														// near plane
			1000.0f														// far plane
			);

		gameObjects.push_back(Base::Objects::GameObject());
		Renderer::BaseObjects::InitSphereMesh(&gameObjects.back().mesh);
		myroam.GenerateCube(&gameObjects.back().mesh->vertices, 4);
		//myroam.Split(0.2f);
		//myroam.Merge(0.6f);
		gameObjects.back().mesh->indices = myroam.GetIndices();
		mRenderer->SetObjectsInScene(&gameObjects);

		mTextRenderer = std::unique_ptr<Renderer::TextRenderer>(new Renderer::TextRenderer(deviceResources));

		mInput->Initialize();

		// Maybe have these in an initialization file?
		float movementSpeed = 0.1f;
		float roatationSpeed = 0.025f;
		mInput->AddInput(
			"WalkForward",
			Input::ValueTypes::FLOAT,
			movementSpeed,
			Windows::System::VirtualKey::W
			);
		mInput->AddInput(
			"WalkBackward",
			Input::ValueTypes::FLOAT,
			movementSpeed,
			Windows::System::VirtualKey::S
			);
		mInput->AddInput(
			"StrafeRight",
			Input::ValueTypes::FLOAT,
			movementSpeed,
			Windows::System::VirtualKey::D
			);
		mInput->AddInput(
			"StrafeLeft",
			Input::ValueTypes::FLOAT,
			movementSpeed,
			Windows::System::VirtualKey::A
			);

		mInput->AddInput(
			"YawRight",
			Input::ValueTypes::FLOAT,
			roatationSpeed,
			Windows::System::VirtualKey::Right
			);
		mInput->AddInput(
			"YawLeft",
			Input::ValueTypes::FLOAT,
			roatationSpeed,
			Windows::System::VirtualKey::Left
			);

		mInput->AddInput(
			"PitchUp",
			Input::ValueTypes::FLOAT,
			roatationSpeed,
			Windows::System::VirtualKey::Up
			);
		mInput->AddInput(
			"PitchDown",
			Input::ValueTypes::FLOAT,
			roatationSpeed,
			Windows::System::VirtualKey::Down
			);

		mInput->AddInput(
			"level1",
			Input::ValueTypes::FLOAT,
			0.5f,
			Windows::System::VirtualKey::Number1
			);
		mInput->AddInput(
			"level2",
			Input::ValueTypes::FLOAT,
			0.25f,
			Windows::System::VirtualKey::Number2
			);
		mInput->AddInput(
			"level3",
			Input::ValueTypes::FLOAT,
			0.125f,
			Windows::System::VirtualKey::Number3
			);
		mInput->AddInput(
			"level4",
			Input::ValueTypes::FLOAT,
			0.0625f,
			Windows::System::VirtualKey::Number4
			);
		
		mPhysics = std::unique_ptr<Physics::Physics>(new Physics::Physics());
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

	// Notifies renderers that device resources need to be released.
	void Engine::OnDeviceLost()
	{
		mRenderer->ReleaseDeviceDependentResources();
		mTextRenderer->ReleaseDeviceDependentResources();
	}

	// Notifies renderers that device resources may now be recreated.
	void Engine::OnDeviceRestored()
	{
		mRenderer->CreateDeviceDependentResources();
		mTextRenderer->CreateDeviceDependentResources();
		mRenderer->CreateWindowSizeDependentResources();
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

		float lvl1, lvl2, lvl3, lvl4;
		lvl1 = mInput->GetInput("level1").fval;
		lvl2 = mInput->GetInput("level2").fval;
		lvl3 = mInput->GetInput("level3").fval;
		lvl4 = mInput->GetInput("level4").fval;

		myroam.Split(1.0f - lvl1 - lvl2 - lvl3 - -lvl4);
		gameObjects.back().mesh->indices = myroam.GetIndices();
		mRenderer->SetObjectsInScene(&gameObjects);

		//gameObjects[0].position = XMVectorSet((float)cos(SimulationTime) * 3.0f,(float)sin(SimulationTime) * 3.0f,0,0);

		mPhysics->Update();
		mRenderer->Update();
		mRenderer->Render();
		Platform::String^ renderString = L"How long can I make this string?";
		POINT mypoint = POINT();
		mypoint.x = 100 + (long)cos(SimulationTime)*100;
		mypoint.y = 100 + (long)sin(SimulationTime) * 100;
		mTextRenderer->Render(renderString, mypoint);

		deviceResources->Present();
		return;
	}

	Input::Input^ Engine::GetInput()
	{
		return mInput;
	}
}