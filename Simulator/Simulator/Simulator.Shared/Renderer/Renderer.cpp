#include "pch.h"
#include "Renderer.h"

namespace Renderer
{

	bool Renderer::Initialize()
	{
		// Define temporary pointers to a device and a device context
		ComPtr<ID3D11Device> dev11;
		ComPtr<ID3D11DeviceContext> devcon11;

		// Create the device and device context objects
		D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			0,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&dev11,
			nullptr,
			&devcon11);

		// Convert the pointers from the DirectX 11 versions to the DirectX 11.1 versions
		dev11.As(&dev);
		devcon11.As(&devcon);

		// First, convert our ID3D11Device2 into an IDXGIDevice2
		ComPtr<IDXGIDevice2> dxgiDevice;
		dev.As(&dxgiDevice);

		// Second, use the IDXGIDevice2 interface to get access to the adapter
		ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetAdapter(&dxgiAdapter);

		// Third, use the IDXGIAdapter interface to get access to the factory
		ComPtr<IDXGIFactory2> dxgiFactory;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), &dxgiFactory);

		// set up the swap chain description
		DXGI_SWAP_CHAIN_DESC1 scd = { 0 };
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;    // how the swap chain should be used
		scd.BufferCount = 2;                                  // a front buffer and a back buffer
		scd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;              // the most common swap chain format
		scd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;    // the recommended flip mode
		scd.SampleDesc.Count = 1;                             // disable anti-aliasing
 
		CoreWindow^ Window = CoreWindow::GetForCurrentThread();    // get the window pointer

		dxgiFactory->CreateSwapChainForCoreWindow(
			dev.Get(),                                  // address of the device
			reinterpret_cast<IUnknown*>(Window),        // address of the window
			&scd,                                       // address of the swap chain description
			nullptr,                                    // advanced
			&swapchain);                                // address of the new swap chain pointer

		// TO RENDER TO THE BACK BUFFER
		// get a pointer directly to the back buffer
		ComPtr<ID3D11Texture2D> backbuffer;
		swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);

		// create a render target pointing to the back buffer
		dev->CreateRenderTargetView(backbuffer.Get(), nullptr, &rendertarget);

		return true;
	}

	bool Renderer::UnInitialize()
	{
		return true;
	}

	void Renderer::Update()
	{
		return;
	}

	void Renderer::Render()
	{
		// set our new render target object as the active render target
		devcon->OMSetRenderTargets(1, rendertarget.GetAddressOf(), nullptr);
		// can use this to set multiple render targets (number, list)

		// clear the back buffer to a deep blue
		float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
		devcon->ClearRenderTargetView(rendertarget.Get(), color);

		swapchain->Present(1, 0);	// swap the back buffer and the front buffer
		return;
	}

}