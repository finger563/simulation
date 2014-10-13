#pragma once

#include "..\Base\Base.h"

#include "Helper.h"
#include "Objects\BaseObjects.h"
#include "Camera\Camera.h"
#include "Lighting\Light.h"
#include "Shaders\Shader.h"
#include "Device Resources\DeviceResources.h"
#include "Text\TextRenderer.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;

namespace Renderer
{
	interface class IRenderer : public Base::ISubsystem
	{
		virtual void Render() = 0;
	};

	ref class Renderer : public IRenderer
	{
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void Render();

	internal: // only used by code in this project
		void SetCamera(
			Vector<3, float> position,
			Vector<3, float> view,
			Vector<3, float> up,
			float FoVY,
			float AspectRatio,
			float NearPlane,
			float FarPlane
			);
		Camera* GetCameraPtr() { return &camera; }
		void SetObjectsInScene(std::vector<Base::Objects::GameObject<float>>* _objects);

	private:
		void InitStates();								// For initializing the rasterizer etc. states.

		Camera camera;									// Camera used to render the scene
		std::vector<Base::Objects::GameObject<float>>* objects;	// pointer to objects created and maintained by Engine

		Shader shader;									// shader to be applied

		std::vector<DirectionalLight> directionalLights;// Vector containing all directional lights in a scene
		std::vector<PointLight> pointLights;			// Vector containing all point lights in a scene
		std::vector<SpotLight> spotLights;				// Vector containing all spot lights in a scene

		ComPtr<ID3D11Buffer> vertexbuffer;				// Dx11.2 GPU vertex buffer interface
		ComPtr<ID3D11Buffer> indexbuffer;				// Dx11.2 GPU index buffer interface
		int numIndices;									// number of indices to draw

		ComPtr<ID3D11RasterizerState> defaultrasterizerstate;		// Rasterizer state (default)
		ComPtr<ID3D11RasterizerState> wireframerasterizerstate;		// Rasterizer state (wireframe)

		//std::shared_ptr<DeviceResources> deviceResources;

		// Everything below here is now contained within device resources

		ComPtr<ID3D11Device2> dev;						// Dx11.2 device interface
		ComPtr<ID3D11DeviceContext2> devcon;			// Dx11.2 device context interface

		ComPtr<IDXGISwapChain1> swapchain;				// Dx11.2 swap chain interface
		ComPtr<ID3D11RenderTargetView> rendertarget;	// Dx11.2 render target interface

		ComPtr<ID3D11DepthStencilView> zbuffer;			// Dx11.2 z-buffer interface
	};
}