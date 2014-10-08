#include "pch.h"
#include "Renderer.h"
#include <fstream>

namespace Renderer
{
	bool Renderer::Initialize()
	{
		time = 0.0f;

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
		viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
		viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

		devcon->RSSetViewports(1, &viewport);

		// enabling Depth Buffering
		D3D11_TEXTURE2D_DESC texd = { 0 };

		texd.Width = (UINT)(Window->Bounds.Width);
		texd.Height = (UINT)(Window->Bounds.Height);
		texd.ArraySize = 1;
		texd.MipLevels = 1;
		texd.SampleDesc.Count = 1;
		texd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

		ComPtr<ID3D11Texture2D> zbuffertexture;
		dev->CreateTexture2D(&texd, nullptr, &zbuffertexture);

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));

		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

		dev->CreateDepthStencilView(zbuffertexture.Get(), &dsvd, &zbuffer);

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
		time += 0.1f;
	}

	void Renderer::Render()
	{
		// set our new render target object as the active render target
		devcon->OMSetRenderTargets(1, rendertarget.GetAddressOf(), zbuffer.Get());
		// can use this to set multiple render targets (number, list)

		// clear the depth buffer
		devcon->ClearDepthStencilView(zbuffer.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		// clear the back buffer to a deep blue
		float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
		devcon->ClearRenderTargetView(rendertarget.Get(), color);

		// set the vertex buffer
		UINT stride = sizeof(BaseVertex<float>);
		UINT offset = 0;
		devcon->IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), &stride, &offset);
		// set the index buffer
		devcon->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		// set the primitive topology
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// THIS SHOULD BE PART OF THE OBJECT'S CODE I THINK? OR A HELPER FUNCTION USING OBJECT PROPERTIES
		XMMATRIX matTranslate = XMMatrixTranslation(0.0f, 0.0f, sinf(time / 2.0f)*3.0f);
		XMMATRIX matScale = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		XMMATRIX matRotateX = XMMatrixRotationX(time / 5.0f);
		XMMATRIX matRotateY = XMMatrixRotationY(time / 5.0f);
		XMMATRIX matRotateZ = XMMatrixRotationZ(XMConvertToRadians(0.0f));
		XMMATRIX matRotation = matRotateX * matRotateY * matRotateZ;
		XMMATRIX matWorld = matRotation * matScale * matTranslate;
		// END OF STUFF THAT SHOULD BE PART OF OBJECT'S CODE

		camera.UpdateMatrices();
		XMMATRIX matFinal = matWorld * camera.ViewMatrix * camera.ProjMatrix;

		CBuffer cbuffer;
		cbuffer.matWVP = matFinal;
		// REQUIRED FOR LIGHTING
		cbuffer.matRotation = matRotation;	// need to get from object
		cbuffer.DiffuseVector = XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f);
		cbuffer.DiffuseColor = XMVectorSet(0.5f, 0.5f, 0.5f, 1.0f);
		cbuffer.AmbientColor = XMVectorSet(0.2f, 0.2f, 0.2f, 1.0f);
		// END REQUIRED FOR LIGHTING

		// set the new values for the constant buffer
		devcon->UpdateSubresource(constantbuffer.Get(), 0, 0, &cbuffer, 0, 0);

		// draw 3 vertices, starting from vertex 0
		devcon->DrawIndexed(36, 0, 0);

		swapchain->Present(1, 0);	// swap the back buffer and the front buffer
		return;
	}

	void Renderer::SetCamera(
		Vector<3, float> position,
		Vector<3, float> view,
		Vector<3, float> up,
		float FoVY,
		float AspectRatio,
		float NearPlane,
		float FarPlane
		)
	{
		camera.Position = XMVectorSet(position[0],position[1],position[2],0);
		camera.View = XMVectorSet(view[0], view[1], view[2], 0);
		camera.Up = XMVectorSet(up[0], up[1], up[2], 0);
		camera.Right = XMVector3Cross(-camera.View, camera.Up);
		camera.FoVY = FoVY;
		camera.Aspect = AspectRatio;
		camera.NearPlane = NearPlane;
		camera.FarPlane = FarPlane;
	}

	void Renderer::Walk(float Dist)
	{
		camera.Walk(Dist);
	}

	void Renderer::Strafe(float Dist)
	{
		camera.Strafe(Dist);
	}

	void Renderer::Pitch(float Angle)
	{
		camera.RotateAroundRight(Angle);
	}

	void Renderer::Roll(float Angle)
	{
		camera.RotateAroundView(Angle);
	}

	void Renderer::Yaw(float Angle)
	{
		camera.RotateAroundUp(Angle);
	}

	void Renderer::InitGraphics()
	{
		BaseVertex<float> OurVertices[] = {
				{ -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f },    // side 1
				{ 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
				{ -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f },

				{ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },    // side 2
				{ -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
				{ 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f },
				{ 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f },

				{ -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f },    // side 3
				{ -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },
				{ 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f },
				{ 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f },

				{ -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },    // side 4
				{ 1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f },
				{ -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f },
				{ 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f },

				{ 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f },    // side 5
				{ 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f },
				{ 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f },
				{ 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f },

				{ -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f },    // side 6
				{ -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
				{ -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f },
				{ -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f },
		};
		
		// create the vertex buffer
		D3D11_BUFFER_DESC vertexBD = { 0 };
		vertexBD.ByteWidth = sizeof(BaseVertex<float>) * ARRAYSIZE(OurVertices);
		vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSRD = { OurVertices, 0, 0 };

		dev->CreateBuffer(&vertexBD, &vertexSRD, &vertexbuffer);
		
		short OurIndices[] =
		{
			0, 1, 2,    // side 1
			2, 1, 3,
			4, 5, 6,    // side 2
			6, 5, 7,
			8, 9, 10,    // side 3
			10, 9, 11,
			12, 13, 14,    // side 4
			14, 13, 15,
			16, 17, 18,    // side 5
			18, 17, 19,
			20, 21, 22,    // side 6
			22, 21, 23,
		};

		// create the index buffer
		// buffer description
		D3D11_BUFFER_DESC indexBD = { 0 };
		indexBD.ByteWidth = sizeof(short) * ARRAYSIZE(OurIndices);    // indices are stored in short values
		indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;

		// subresource data
		D3D11_SUBRESOURCE_DATA indexSRD = { OurIndices, 0, 0 };

		dev->CreateBuffer(&indexBD, &indexSRD, &indexbuffer);
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
			//{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		// create the input layout
		dev->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile->Data, VSFile->Length, &inputlayout);
		devcon->IASetInputLayout(inputlayout.Get());

		// NEEDS TO BE MOVED TO SHADER SUBSYSTEM
		D3D11_BUFFER_DESC bd = { 0 };

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(CBuffer);		// must always be a multiple of 16 bytes
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		dev->CreateBuffer(&bd, nullptr, &constantbuffer);
		devcon->VSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
		// WOULD NEED TO USE PSSetConstantBuffers IF THE PS NEEDED IT
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