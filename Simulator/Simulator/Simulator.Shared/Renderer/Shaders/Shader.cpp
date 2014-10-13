#include "pch.h"
#include "Shader.h"
#include <fstream>

using namespace Renderer;

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;

Shader::Shader(const std::shared_ptr<DeviceResources>& devResources) :
deviceResources(devResources)
{

}

void Shader::Initialize()
{
	// load the shader files
	Platform::Array<byte>^ VSFile = LoadShaderFile("VertexShader.cso");
	Platform::Array<byte>^ PSFile = LoadShaderFile("PixelShader.cso");

	// create the shader objects
	deviceResources->GetD3DDevice()->CreateVertexShader(VSFile->Data, VSFile->Length, nullptr, &vertexshader);
	deviceResources->GetD3DDevice()->CreatePixelShader(PSFile->Data, PSFile->Length, nullptr, &pixelshader);


	// initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create the input layout
	deviceResources->GetD3DDevice()->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile->Data, VSFile->Length, &inputlayout);

	// initialize the constant buffer
	D3D11_BUFFER_DESC bd = { 0 };

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CBuffer);		// must always be a multiple of 16 bytes
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	// create the constant buffer
	deviceResources->GetD3DDevice()->CreateBuffer(&bd, nullptr, &constantbuffer);
}

void Shader::UnInitialize()
{

}

void Shader::Apply()
{
	// set the input layout as the active input layout
	deviceResources->GetD3DDeviceContext()->IASetInputLayout(inputlayout.Get());
	// set the shader objects as the active shaders
	deviceResources->GetD3DDeviceContext()->VSSetShader(vertexshader.Get(), nullptr, 0);
	deviceResources->GetD3DDeviceContext()->PSSetShader(pixelshader.Get(), nullptr, 0);
	// set the constant buffer for the vertex and pixel shaders
	deviceResources->GetD3DDeviceContext()->VSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
	deviceResources->GetD3DDeviceContext()->PSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
}

// this function loads a file into an Array^
Platform::Array<byte>^ Renderer::LoadShaderFile(std::string File)
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