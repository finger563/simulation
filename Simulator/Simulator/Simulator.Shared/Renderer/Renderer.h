#pragma once

#include "Base\Subsystem.h"
#include "Base\Math\Vector.h"
#include "Base\Math\Matrix.h"

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
	private:
		ComPtr<ID3D11Device2> dev;						// Dx11.2 device interface
		ComPtr<ID3D11DeviceContext2> devcon;			// Dx11.2 device context interface
		ComPtr<IDXGISwapChain1> swapchain;				// Dx11.2 swap chain interface
		ComPtr<ID3D11RenderTargetView> rendertarget;	// Dx11.2 render target interface
	public:
		virtual bool Initialize();
		virtual void Update();
		virtual bool UnInitialize();

		virtual void Render();
	};
}