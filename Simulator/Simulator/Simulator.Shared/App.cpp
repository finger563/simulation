// Include the precompiled headers
#include "pch.h"
#include "App.h"

#include <ppltasks.h>

using namespace concurrency;
// Use some common namespaces to simplify the code
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Popups;
using namespace Windows::System;
using namespace Windows::Foundation;
using namespace Windows::Graphics::Display;
using namespace Platform;

// the starting point of all programs
[MTAThread]    // define main() as a multi-threaded-apartment function
int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new Direct3DApplicationSource());    // create and run a new AppSource class
	return 0;
}

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new App::App();
}


App::App::App() :
WindowClosed(false),
WindowVisible(true)
{
}

// some functions called by Windows
void App::App::Initialize(CoreApplicationView^ AppView)
{
	// set the OnActivated function to handle to Activated "event"
	AppView->Activated += ref new TypedEventHandler
		<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);

	CoreApplication::Suspending += ref new EventHandler
		<SuspendingEventArgs^>(this, &App::OnSuspending);
	CoreApplication::Resuming += ref new EventHandler
		<Object^>(this, &App::OnResuming);

	WindowClosed = false;

	mEngine = ref new Engine::Engine();
}

void App::App::SetWindow(CoreWindow^ Window)
{
	Window->Closed += ref new TypedEventHandler
		<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnClosed);

	Window->PointerPressed += ref new TypedEventHandler
		<CoreWindow^, PointerEventArgs^>(mEngine->mInput, &Input::Input::PointerPressed);
	Window->KeyDown += ref new TypedEventHandler
		<CoreWindow^, KeyEventArgs^>(mEngine->mInput, &Input::Input::KeyDown);
	Window->KeyUp += ref new TypedEventHandler
		<CoreWindow^, KeyEventArgs^>(mEngine->mInput, &Input::Input::KeyUp);
	Window->PointerWheelChanged += ref new TypedEventHandler
		<CoreWindow^, PointerEventArgs^>(mEngine->mInput, &Input::Input::PointerWheelChanged);

	mEngine->SetWindowProperties(Window->Bounds.Width, Window->Bounds.Height);
}

void App::App::Load(String^ EntryPoint) {}

void App::App::Run()
{
	mEngine->Initialize(); // INITIALIZE THE SIMULATOR

	// Obtain a pointer to the window
	CoreWindow^ Window = CoreWindow::GetForCurrentThread();

	while (!WindowClosed)
	{
		Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

		mEngine->Update();		// HANDLES EVERYTHING FOR THE SIMULATOR
	}
}

void App::App::Uninitialize() {}

// an "event" that is called when the application window is ready to be activated
void App::App::OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
{
	CoreWindow^ Window = CoreWindow::GetForCurrentThread();
	Window->Activate();
}

void App::App::OnSuspending(Object^ Sender, SuspendingEventArgs^ Args) {} // Save the state of the simulator here

void App::App::OnResuming(Object^ Sender, Object^ Args) {} // Load the state of the simulator here

void App::App::OnClosed(CoreWindow^ Sender, CoreWindowEventArgs^ Args)
{
	WindowClosed = true;
}