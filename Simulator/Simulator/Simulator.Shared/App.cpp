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
using namespace Windows::UI::Input;
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

	mEngine = std::unique_ptr<Engine::Engine>(new Engine::Engine());
}

void App::App::SetWindow(CoreWindow^ Window)
{
	mEngine->mWindow = Window;

	// Window Handlers

	Window->Closed += ref new TypedEventHandler
		<CoreWindow^, CoreWindowEventArgs^>(this, &App::OnWindowClosed);

	Window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &App::OnVisibilityChanged);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
	Window->SizeChanged +=
		ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &App::OnWindowSizeChanged);

	// DisplayInformation Event Handlers

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnOrientationChanged);

	// Disable all pointer visual feedback for better performance when touching.
	// This is not supported on Windows Phone applications.
	auto pointerVisualizationSettings = PointerVisualizationSettings::GetForCurrentView();
	pointerVisualizationSettings->IsContactFeedbackEnabled = false;
	pointerVisualizationSettings->IsBarrelButtonFeedbackEnabled = false;
#endif

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &App::OnDisplayContentsInvalidated);

	// Input Handlers

	Window->PointerPressed += ref new TypedEventHandler
		<CoreWindow^, PointerEventArgs^>(mEngine->mInput, &Input::Input::PointerPressed);
	Window->KeyDown += ref new TypedEventHandler
		<CoreWindow^, KeyEventArgs^>(mEngine->mInput, &Input::Input::KeyDown);
	Window->KeyUp += ref new TypedEventHandler
		<CoreWindow^, KeyEventArgs^>(mEngine->mInput, &Input::Input::KeyUp);
	Window->PointerWheelChanged += ref new TypedEventHandler
		<CoreWindow^, PointerEventArgs^>(mEngine->mInput, &Input::Input::PointerWheelChanged);

	// should set window here? for the renderer's deviceResources?
}

void App::App::Load(String^ EntryPoint) 
{
	if (mEngine == nullptr)
	{
		mEngine = std::unique_ptr<Engine::Engine>(new Engine::Engine());
	}
}

void App::App::Run()
{
	mEngine->Initialize(); // INITIALIZE THE SIMULATOR

	while (!WindowClosed)
	{
		if (WindowVisible)
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

			mEngine->Update();		// HANDLES EVERYTHING FOR THE SIMULATOR
		}
		else
		{
			CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
		}
	}
}

void App::App::Uninitialize() {}

// an event that is called when the application window is ready to be activated
void App::App::OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
{
	// Run() won't start until the CoreWindow is activated.
	CoreWindow::GetForCurrentThread()->Activate();
}

// Save the state of the simulator here
void App::App::OnSuspending(Object^ Sender, SuspendingEventArgs^ Args) 
{
	// Save app state asynchronously after requesting a deferral. Holding a deferral
	// indicates that the application is busy performing suspending operations. Be
	// aware that a deferral may not be held indefinitely. After about five seconds,
	// the app will be forced to exit.
	SuspendingDeferral^ deferral = Args->SuspendingOperation->GetDeferral();

	create_task([this, deferral]()
	{
		mEngine->OnSuspending();

		deferral->Complete();
	});
}

// Load the state of the simulator here
void App::App::OnResuming(Object^ Sender, Object^ Args) 
{
	// Restore any data or state that was unloaded on suspend. By default, data
	// and state are persisted when resuming from suspend. Note that this event
	// does not occur if the app was previously terminated.

	// Insert your code here.
	mEngine->OnResuming();
}

// Window event handlers.

void App::App::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ Args)
{
	WindowVisible = Args->Visible;
}

void App::App::OnWindowClosed(CoreWindow^ Sender, CoreWindowEventArgs^ Args)
{
	WindowClosed = true;
}

#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
void App::App::OnWindowSizeChanged(CoreWindow^ sender, WindowSizeChangedEventArgs^ args)
{
	mEngine->mRenderer->OnWindowSizeChanged(sender, args);
}
#endif

// DisplayInformation event handlers.

void App::App::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	mEngine->mRenderer->OnDisplayContentsInvalidated(sender, args);
}

#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
void App::App::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	mEngine->mRenderer->OnDpiChanged(sender, args);
}

void App::App::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	mEngine->mRenderer->OnOrientationChanged(sender, args);
}
#endif