#pragma once

#include "Base\Subsystem.h"
#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"
#include "Vertex.h"
#include "Camera\Camera.h"

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
		void SetCamera(
			Vector<3, double> position,
			Vector<3, double> view,
			Vector<3, double> up,
			float FoVY,
			float AspectRatio,
			float NearPlane,
			float FarPlane
			);

	private:
		float time;
		Camera camera;

		ComPtr<ID3D11Device2> dev;						// Dx11.2 device interface
		ComPtr<ID3D11DeviceContext2> devcon;			// Dx11.2 device context interface

		ComPtr<IDXGISwapChain1> swapchain;				// Dx11.2 swap chain interface
		ComPtr<ID3D11RenderTargetView> rendertarget;	// Dx11.2 render target interface

		ComPtr<ID3D11DepthStencilView> zbuffer;			// Dx11.2 z-buffer interface

		ComPtr<ID3D11Buffer> vertexbuffer;				// Dx11.2 GPU vertex buffer interface
		ComPtr<ID3D11Buffer> indexbuffer;				// Dx11.2 GPU index buffer interface
		ComPtr<ID3D11InputLayout> inputlayout;

		ComPtr<ID3D11VertexShader> vertexshader;		// should be moved to shader subsystem
		ComPtr<ID3D11PixelShader> pixelshader;			// should be moved to shader subsystem

		ComPtr<ID3D11Buffer> constantbuffer;			// one of the constant buffers : should be moved to separate (shader?) subsystem
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		void InitGraphics();
		// this function initializes the GPU settings and prepares it for rendering
		void InitPipeline();

		virtual void Render();

	};
}