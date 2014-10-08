#pragma once

#include "..\Base\Base.h"

#include "Objects\BaseObjects.h"
#include "Vertex.h"
#include "Camera\Camera.h"
#include "Lighting\Light.h"
#include "Shaders\ConstantBuffers.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;

namespace Renderer
{
	interface class IRenderer : public Base::ISubsystem
	{
		virtual void Render() = 0;
	};

	// this function loads a file into an Array^
	Platform::Array<byte>^ LoadShaderFile(std::string File);

	ref class Renderer : public IRenderer
	{
	internal: // only used by code in this project
		void Walk	(float Dist);
		void Strafe	(float Dist);
		void Pitch	(float Angle);
		void Roll	(float Angle);
		void Yaw	(float Angle);
		void SetCamera(
			Vector<3, float> position,
			Vector<3, float> view,
			Vector<3, float> up,
			float FoVY,
			float AspectRatio,
			float NearPlane,
			float FarPlane
			);
		void SetObjectsInScene(std::vector<Base::Objects::GameObject<float>>* _objects);
	private:
		float time;										// time value (this should be removed)
		Camera camera;									// Camera used to render the scene
		std::vector<Base::Objects::GameObject<float>>* objects;

		std::vector<DirectionalLight> directionalLights;// Vector containing all directional lights in a scene
		std::vector<PointLight> pointLights;			// Vector containing all point lights in a scene
		std::vector<SpotLight> spotLights;				// Vector containing all spot lights in a scene

		ComPtr<ID3D11Device2> dev;						// Dx11.2 device interface
		ComPtr<ID3D11DeviceContext2> devcon;			// Dx11.2 device context interface

		ComPtr<IDXGISwapChain1> swapchain;				// Dx11.2 swap chain interface
		ComPtr<ID3D11RenderTargetView> rendertarget;	// Dx11.2 render target interface

		ComPtr<ID3D11DepthStencilView> zbuffer;			// Dx11.2 z-buffer interface

		ComPtr<ID3D11Buffer> vertexbuffer;				// Dx11.2 GPU vertex buffer interface
		ComPtr<ID3D11Buffer> indexbuffer;				// Dx11.2 GPU index buffer interface
		int numIndices;									// number of indices to draw
		ComPtr<ID3D11InputLayout> inputlayout;

		ComPtr<ID3D11VertexShader> vertexshader;		// should be moved to shader subsystem
		ComPtr<ID3D11PixelShader> pixelshader;			// should be moved to shader subsystem

		ComPtr<ID3D11Buffer> constantbuffer;			// one of the constant buffers : should be moved to separate subsystem (shader?)
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void Render();

		// InitGraphics() initializes the vertex buffer & index buffer : should be moved to separate subsystem
		void InitGraphics();
		// InitPipeline() initializes the GPU shaders, input layout, and constant buffers : should be moved to separate subsystem
		void InitPipeline();
	};
}