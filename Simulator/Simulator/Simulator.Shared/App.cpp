// Include the precompiled headers
#include "pch.h"

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


// the class definition for the core "framework" of our app
ref class App sealed : public IFrameworkView
{
public:
	// some functions called by Windows
	virtual void Initialize(CoreApplicationView^ AppView)
	{
		// set the OnActivated function to handle to Activated "event"
		AppView->Activated += ref new TypedEventHandler
			<CoreApplicationView^, IActivatedEventArgs^>(this, &App::OnActivated);
	}
	virtual void SetWindow(CoreWindow^ Window) 
	{
		Window->PointerPressed += ref new TypedEventHandler
			<CoreWindow^, PointerEventArgs^>(this, &App::PointerPressed);
		Window->KeyDown += ref new TypedEventHandler
			<CoreWindow^, KeyEventArgs^>(this, &App::KeyDown);
		Window->KeyUp += ref new TypedEventHandler
			<CoreWindow^, KeyEventArgs^>(this, &App::KeyUp);
		Window->PointerWheelChanged += ref new TypedEventHandler
			<CoreWindow^, PointerEventArgs^>(this, &App::PointerWheelChanged);
	}
	virtual void Load(String^ EntryPoint) {}
	virtual void Run() 
	{
		// Obtain a pointer to the window
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();

		// Run ProcessEvents() to dispatch events
		Window->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	}
	virtual void Uninitialize() {}

	// an "event" that is called when the application window is ready to be activated
	void OnActivated(CoreApplicationView^ CoreAppView, IActivatedEventArgs^ Args)
	{
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();
		Window->Activate();
	}

	void KeyDown(CoreWindow^ Window, KeyEventArgs^ Args)
	{
		if (Args->VirtualKey == VirtualKey::A ||
			Args->VirtualKey == VirtualKey::S ||
			Args->VirtualKey == VirtualKey::W ||
			Args->VirtualKey == VirtualKey::D)
		{
			// do something..
			MessageDialog Dialog("You pressed a key!", "Notice!");
			Dialog.ShowAsync();
		}
	}

	void KeyUp(CoreWindow^ Window, KeyEventArgs^ Args)
	{
	}

	void PointerPressed(CoreWindow^ Window, PointerEventArgs^ Args)
	{
		MessageDialog Dialog("Thank you for clicking me!", "Notice!");
		Dialog.ShowAsync();
	}

	void PointerWheelChanged(CoreWindow^ Window, PointerEventArgs^ Args)
	{
		int Wheel = Args->CurrentPoint->Properties->MouseWheelDelta; // increments of 120; + is up, - is down
	}
};


// the class definition that creates an instance of our core framework class
ref class AppSource sealed : IFrameworkViewSource
{
public:
	virtual IFrameworkView^ CreateView()
	{
		return ref new App();    // create an App class and return it
	}
};

[MTAThread]    // define main() as a multi-threaded-apartment function

// the starting point of all programs
int main(Array<String^>^ args)
{
	CoreApplication::Run(ref new AppSource());    // create and run a new AppSource class
	return 0;
}