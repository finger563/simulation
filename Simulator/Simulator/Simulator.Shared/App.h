#pragma once

// Include the precompiled headers
#include "pch.h"
#include "Engine\Engine.h"

namespace App
{
	// the class definition for the core "framework" of our app
	ref class App sealed : public Windows::ApplicationModel::Core::IFrameworkView
	{
	private:
		bool WindowClosed;
		bool WindowVisible;
		std::unique_ptr<Engine::Engine> mEngine;
	public:
		App();
		// some functions called by Windows
		virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ AppView);
		virtual void SetWindow(CoreWindow^ Window);
		virtual void Load(Platform::String^ EntryPoint);
		virtual void Run();
		virtual void Uninitialize();

		// an "event" that is called when the application window is ready to be activated
		void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ CoreAppView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ Args);
		// Save the state of the simulator here
		void OnSuspending(Object^ Sender, Windows::ApplicationModel::SuspendingEventArgs^ Args);
		// Load the state of the simulator here
		void OnResuming(Object^ Sender, Object^ Args); 

		// Window Event Handlers
#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ Sender, Windows::UI::Core::WindowSizeChangedEventArgs^ Args);
#endif
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ Sender, Windows::UI::Core::VisibilityChangedEventArgs^ Args);
		void OnWindowClosed(Windows::UI::Core::CoreWindow^ Sender, Windows::UI::Core::CoreWindowEventArgs^ Args);

		// DisplayInformation event handlers.
#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ Sender, Platform::Object^ Args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ Sender, Platform::Object^ Args);
#endif
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ Sender, Platform::Object^ Args);
	};
}

// the class definition that creates an instance of our core framework class
ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
