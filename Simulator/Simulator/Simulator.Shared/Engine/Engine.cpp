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
			Base::Math::VectorInit({ 0.0, 1.0, -5.0, 1.0 }),			// position
			Base::Math::VectorInit({ 0.0, 0.0, 1.0, 1.0 }),				// view
			Base::Math::VectorInit({ 0.0, 1.0, 0.0, 1.0 }),				// up
			45.0f,														// FOVY
			(float)(mWindow->Bounds.Width / mWindow->Bounds.Height),	// aspect ratio
			1.0f,														// near plane
			1000.0f														// far plane
			);

		gameObjects.push_back(Base::Objects::GameObject());
		Renderer::BaseObjects::InitSphereMesh(&gameObjects.back().mesh);
		myroam.GenerateCube(&gameObjects.back().mesh->vertices, 7);

		mTextRenderer = std::unique_ptr<Renderer::TextRenderer>(new Renderer::TextRenderer(deviceResources));

		mInput->Initialize();

		mInput->AddInput(
			"setUpdatePGM",
			Windows::System::VirtualKey::U,
			Input::ValueTypes::FLOAT,
			1.0
			);
		mInput->AddInput(
			"unsetUpdatePGM",
			Windows::System::VirtualKey::O,
			Input::ValueTypes::FLOAT,
			1.0
			);
		mInput->AddInput(
			"IncreasePGMX",
			Windows::System::VirtualKey::L,
			Input::ValueTypes::FLOAT,
			1.0
			);
		mInput->AddInput(
			"DecreasePGMX",
			Windows::System::VirtualKey::J,
			Input::ValueTypes::FLOAT,
			1.0
			);
		mInput->AddInput(
			"IncreasePGMY",
			Windows::System::VirtualKey::I,
			Input::ValueTypes::FLOAT,
			1.0
			);
		mInput->AddInput(
			"DecreasePGMY",
			Windows::System::VirtualKey::K,
			Input::ValueTypes::FLOAT,
			1.0
			);

		// Maybe have these in an initialization file?
		float movementSpeed = 0.1f;
		float roatationSpeed = 0.025f;
		mInput->AddInput(
			"WalkForward",
			Windows::System::VirtualKey::W,
			Input::ValueTypes::FLOAT,
			movementSpeed
			);
		mInput->AddInput(
			"WalkBackward",
			Windows::System::VirtualKey::S,
			Input::ValueTypes::FLOAT,
			movementSpeed
			);
		mInput->AddInput(
			"StrafeRight",
			Windows::System::VirtualKey::D,
			Input::ValueTypes::FLOAT,
			movementSpeed
			);
		mInput->AddInput(
			"StrafeLeft",
			Windows::System::VirtualKey::A,
			Input::ValueTypes::FLOAT,
			movementSpeed
			);

		mInput->AddInput(
			"RotateLeft",
			Windows::System::VirtualKey::Q,
			Input::ValueTypes::FLOAT,
			roatationSpeed
			);
		mInput->AddInput(
			"RotateRight",
			Windows::System::VirtualKey::E,
			Input::ValueTypes::FLOAT,
			roatationSpeed
			);
		mInput->AddInput(
			"YawRight",
			Windows::System::VirtualKey::Right,
			Input::ValueTypes::FLOAT,
			roatationSpeed
			);
		mInput->AddInput(
			"YawLeft",
			Windows::System::VirtualKey::Left,
			Input::ValueTypes::FLOAT,
			roatationSpeed
			);

		mInput->AddInput(
			"PitchUp",
			Windows::System::VirtualKey::Up,
			Input::ValueTypes::FLOAT,
			roatationSpeed
			);
		mInput->AddInput(
			"PitchDown",
			Windows::System::VirtualKey::Down,
			Input::ValueTypes::FLOAT,
			roatationSpeed
			);

		mInput->AddInput(
			"split1",
			Windows::System::VirtualKey::Number1,
			Input::ValueTypes::FLOAT,
			0.5f
			);
		mInput->AddInput(
			"split2",
			Windows::System::VirtualKey::Number2,
			Input::ValueTypes::FLOAT,
			0.25f
			);
		mInput->AddInput(
			"split3",
			Windows::System::VirtualKey::Number3,
			Input::ValueTypes::FLOAT,
			0.125f
			);
		mInput->AddInput(
			"split4",
			Windows::System::VirtualKey::Number4,
			Input::ValueTypes::FLOAT,
			0.0625f
			);


		mInput->AddInput(
			"merge1",
			Windows::System::VirtualKey::Number5,
			Input::ValueTypes::FLOAT,
			0.51f
			);
		mInput->AddInput(
			"merge2",
			Windows::System::VirtualKey::Number6,
			Input::ValueTypes::FLOAT,
			0.251f
			);
		mInput->AddInput(
			"merge3",
			Windows::System::VirtualKey::Number7,
			Input::ValueTypes::FLOAT,
			0.1251f
			);
		mInput->AddInput(
			"merge4",
			Windows::System::VirtualKey::Number8,
			Input::ValueTypes::FLOAT,
			0.06251f
			);

		mInput->AddInput(
			"wireframeRS",
			Windows::System::VirtualKey::R,
			Input::ValueTypes::FLOAT,
			1.0f
			);
		mInput->AddInput(
			"defaultRS",
			Windows::System::VirtualKey::T,
			Input::ValueTypes::FLOAT,
			1.0f
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
		//SimulationTime += 0.05;

		mInput->Update();

		bool setUpdatePGM = (bool)(mInput->GetInput("setUpdatePGM").fval);
		if (setUpdatePGM)
		{
			mRenderer->SetUpdatePGM(true);
		}
		bool unsetUpdatePGM = (bool)(mInput->GetInput("unsetUpdatePGM").fval);
		if (unsetUpdatePGM)
		{
			mRenderer->SetUpdatePGM(false);
		}
		int pgmx = mRenderer->pgm.GetGridPointXSize();
		int pgmy = mRenderer->pgm.GetGridPointYSize();
		bool increasePGMX = (bool)(mInput->GetInput("IncreasePGMX").fval);
		bool decreasePGMX = (bool)(mInput->GetInput("DecreasePGMX").fval);
		bool increasePGMY = (bool)(mInput->GetInput("IncreasePGMY").fval);
		bool decreasePGMY = (bool)(mInput->GetInput("DecreasePGMY").fval);
		if (increasePGMX)
			pgmx++;
		if (decreasePGMX)
			pgmx--;
		if (increasePGMY)
			pgmy++;
		if (decreasePGMY)
			pgmy--;
		bool makeNewGridPoints = false;
		if (pgmx != mRenderer->pgm.GetGridPointXSize())
		{
			mRenderer->pgm.SetGridPointXSize(pgmx);
			makeNewGridPoints = true;
		}
		if (pgmy != mRenderer->pgm.GetGridPointYSize())
		{
			mRenderer->pgm.SetGridPointYSize(pgmy);
			makeNewGridPoints = true;
		}
		if (makeNewGridPoints)
			mRenderer->pgm.MakeGridPoints();

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
		float rotate = 0;
		rotate += mInput->GetInput("RotateLeft").fval;
		rotate -= mInput->GetInput("RotateRight").fval;

		mRenderer->GetCameraPtr()->Pitch(pitch);
		mRenderer->GetCameraPtr()->Yaw(yaw);
		mRenderer->GetCameraPtr()->Walk(walk);
		mRenderer->GetCameraPtr()->Strafe(strafe);
		mRenderer->GetCameraPtr()->Roll(rotate);

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

		if (s1 > 0.0f)
			myroam.Split(s1);
		else if (s2 > 0.0f)
			myroam.Split(s2);
		else if (s3 > 0.0f)
			myroam.Split(s3);
		else if (s4 > 0.0f)
			myroam.Split(s4);

		if (m1 > 0.0f)
			myroam.Merge(m1);
		else if (m2 > 0.0f)
			myroam.Merge(m2);
		else if (m3 > 0.0f)
			myroam.Merge(m3);
		else if (m4 > 0.0f)
			myroam.Merge(m4);

		gameObjects.back().mesh->indices = myroam.GetIndices();
		mRenderer->SetObjectsInScene(&gameObjects);

		if (mInput->GetInput("defaultRS").fval)
			mRenderer->SetRasterizerState("default");
		if (mInput->GetInput("wireframeRS").fval)
			mRenderer->SetRasterizerState("wireframe");

		//gameObjects[0].position = XMVectorSet((float)cos(SimulationTime) * 3.0f,(float)sin(SimulationTime) * 3.0f,0,0);

		mPhysics->Update();
		mRenderer->Update();
		mRenderer->Render();
		Platform::String^ renderString = L"Change Rasterizer state: r,t\n";
		renderString += "Split: 1-4\n";
		renderString += "Merge: 5-8\n";
		renderString += "Movement: w,a,s,d\n";
		renderString += "Look: up, down, left, right\n";
		renderString += "Update Grid points (X/Y): j,l,i,k\n";
		renderString += "enable/disable PGM update: u,o";
		POINT mypoint = POINT();
		mypoint.x = (long)(cos(SimulationTime) * 100.0);
		mypoint.y = 100 + (long)(sin(SimulationTime) * 100.0);
		mTextRenderer->Render(renderString, mypoint);

		deviceResources->Present();
		return;
	}

	Input::Input^ Engine::GetInput()
	{
		return mInput;
	}
}