#include "pch.h"
#include "Renderer.h"
#include <fstream>

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

		// set the viewport
		D3D11_VIEWPORT viewport = { 0 };

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = Window->Bounds.Width;
		viewport.Height = Window->Bounds.Height;

		devcon->RSSetViewports(1, &viewport);

		InitGraphics();
		InitPipeline();

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

		// set the vertex buffer
		UINT stride = sizeof(BaseVertex<float>);
		UINT offset = 0;
		devcon->IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), &stride, &offset);

		// set the primitive topology
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw 3 vertices, starting from vertex 0
		devcon->Draw(3, 0);

		swapchain->Present(1, 0);	// swap the back buffer and the front buffer
		return;
	}

	void Renderer::InitGraphics()
	{
		BaseVertex<float> OurVertices[] = {
			BaseVertex<float>({ 0.0f, 0.5f, 0.0f }),
			BaseVertex<float>({ 0.45f, -0.5f, 0.0f }),
			BaseVertex<float>({ -0.45f, -0.5f, 0.0f })
		};

		D3D11_BUFFER_DESC bd = { 0 };
		bd.ByteWidth = sizeof(BaseVertex<float>) * ARRAYSIZE(OurVertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA srd = { OurVertices, 0, 0 };

		dev->CreateBuffer(&bd, &srd, &vertexbuffer);
	}

	void Renderer::InitPipeline()
	{
		// load the shader files
		Platform::Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");
		Platform::Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");

		// create the shader objects
		dev->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexshader);
		dev->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelshader);

		// set the shader objects as the active shaders
		devcon->VSSetShader(vertexshader.Get(), nullptr, 0);
		devcon->PSSetShader(pixelshader.Get(), nullptr, 0);

		// initialize input layout
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// create the input layout
		dev->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile->Data, VSFile->Length, &inputlayout);
		devcon->IASetInputLayout(inputlayout.Get());
	}

	// this function loads a file into an Array^
	Platform::Array<byte>^ LoadShaderFile(std::string File)
	{
		Platform::Array<byte>^ FileData = nullptr;

		// open the file
		std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

		// if open was successful
		if (VertexFile.is_open())
		{
			// find the length of the file
			int Length = (int)VertexFile.tellg();

			// collect the file data
			FileData = ref new Platform::Array<byte>(Length);
			VertexFile.seekg(0, std::ios::beg);
			VertexFile.read(reinterpret_cast<char*>(FileData->Data), Length);
			VertexFile.close();
		}

		return FileData;
	}
}