#pragma once

#include "..\Base\Base.h"

#include "Helper.h"
#include "Objects\BaseObjects.h"
#include "Camera\Camera.h"
#include "Lighting\Light.h"
#include "Shaders\Shader.h"
#include "Device Resources\DeviceResources.h"
#include "Text\TextRenderer.h"
#include "Objects\ROAM.h"
#include "PGM\PGM.h"

namespace Renderer
{
	interface IRenderer : public Base::ISubsystem
	{
		virtual void Render() = 0;
		virtual void CreateWindowSizeDependentResources() = 0;
		virtual void SetRasterizerState(Platform::String^ state) = 0;
	};

	class Renderer : public IRenderer
	{
	public:
		Renderer(const std::shared_ptr<DeviceResources>& deviceResources);
		virtual bool Initialize();
		virtual void CreateWindowSizeDependentResources();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void SetRasterizerState(Platform::String^ state);

		virtual void Render();

		virtual void OnSuspending();
		virtual void OnResuming();

		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();

#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		virtual void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ Sender, Windows::UI::Core::WindowSizeChangedEventArgs^ Args);
#endif
		virtual void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ Sender, Platform::Object^ Args);
#if !(WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
		virtual void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ Sender, Platform::Object^ Args);
		virtual void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ Sender, Platform::Object^ Args);
#endif

		void SetUpdatePGM(Platform::Boolean update);
		Platform::Boolean GetUpdatePGM();

		// must overload these for now because
		// (1) we are creating engine=>renderer using unique_ptr
		// (2) the XMVECTOR/XMMATRIX must be 16 byte aligned
		void* operator new(size_t);
		void operator delete(void*);

		void SetCamera(
			Vector position,
			Vector view,
			Vector up,
			float FoVY,
			float AspectRatio,
			float NearPlane,
			float FarPlane
			);
		Camera* GetCameraPtr() { return &camera; }
		void SetObjectsInScene(std::vector<Base::Objects::GameObject>* _objects);

	private:
		void InitStates();								// For initializing the rasterizer etc. states.

		PGM pgm;
		bool updatePGM;

		Camera camera;									// Camera used to render the scene
		std::vector<Base::Objects::GameObject>* objects;	// pointer to objects created and maintained by Engine

		std::unique_ptr<Shader> shader;									// shader to be applied

		std::vector<DirectionalLight> directionalLights;// Vector containing all directional lights in a scene
		std::vector<PointLight> pointLights;			// Vector containing all point lights in a scene
		std::vector<SpotLight> spotLights;				// Vector containing all spot lights in a scene

		ComPtr<ID3D11Buffer> vertexbuffer;				// Dx11.2 GPU vertex buffer interface
		ComPtr<ID3D11Buffer> indexbuffer;				// Dx11.2 GPU index buffer interface
		int numIndices;									// number of indices to draw

		ComPtr<ID3D11RasterizerState> defaultrasterizerstate;		// Rasterizer state (default)
		ComPtr<ID3D11RasterizerState> wireframerasterizerstate;		// Rasterizer state (wireframe)

		std::shared_ptr<DeviceResources> deviceResources;
	};
}