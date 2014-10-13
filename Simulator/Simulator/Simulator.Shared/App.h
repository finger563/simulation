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
		Engine::Engine^ mEngine;
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
		// Save the state of the simulator here
		void OnClosed(CoreWindow^ Sender, CoreWindowEventArgs^ Args);

		// Window event handlers.
#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
#endif
		void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
		void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);

		// DisplayInformation event handlers.
#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
		void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
#endif
		void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
	};
}

// the class definition that creates an instance of our core framework class
ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
{
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
