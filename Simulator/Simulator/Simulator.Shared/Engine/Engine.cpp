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
			"split1",
			Input::ValueTypes::FLOAT,
			0.5f,
			Windows::System::VirtualKey::Number1
			);
		mInput->AddInput(
			"split2",
			Input::ValueTypes::FLOAT,
			0.25f,
			Windows::System::VirtualKey::Number2
			);
		mInput->AddInput(
			"split3",
			Input::ValueTypes::FLOAT,
			0.125f,
			Windows::System::VirtualKey::Number3
			);
		mInput->AddInput(
			"split4",
			Input::ValueTypes::FLOAT,
			0.0625f,
			Windows::System::VirtualKey::Number4
			);


		mInput->AddInput(
			"merge1",
			Input::ValueTypes::FLOAT,
			0.51f,
			Windows::System::VirtualKey::Number5
			);
		mInput->AddInput(
			"merge2",
			Input::ValueTypes::FLOAT,
			0.251f,
			Windows::System::VirtualKey::Number6
			);
		mInput->AddInput(
			"merge3",
			Input::ValueTypes::FLOAT,
			0.1251f,
			Windows::System::VirtualKey::Number7
			);
		mInput->AddInput(
			"merge4",
			Input::ValueTypes::FLOAT,
			0.06251f,
			Windows::System::VirtualKey::Number8
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

		float s1, s2, s3, s4;
		s1 = mInput->GetInput("split1").fval;
		s2 = mInput->GetInput("split2").fval;
		s3 = mInput->GetInput("split3").fval;
		s4 = mInput->GetInput("split4").fval;
		float m1, m2, m3, m4;
		m1 = mInput->GetInput("merge1").fval;
		m2 = mInput->GetInput("merge2").fval;
		m3 = mInput->GetInput("merge3").fval;
		m4 = mInput->GetInput("merge4").fval;

		if (s1)
			myroam.Split(s1);
		else if (s2)
			myroam.Split(s2);
		else if (s3)
			myroam.Split(s3);
		else if (s4)
			myroam.Split(s4);

		if (m1)
			myroam.Merge(m1);
		else if (m2)
			myroam.Merge(m2);
		else if (m3)
			myroam.Merge(m3);
		else if (m4)
			myroam.Merge(m4);

		//myroam.Merge(0.6f);
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