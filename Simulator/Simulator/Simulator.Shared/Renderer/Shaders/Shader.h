#pragma once

#include "pch.h"

#include "ConstantBuffers.h"
#include "..\Helper.h"
#include "..\Device Resources\DeviceResources.h"

using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace DirectX;

namespace Renderer
{
	// this function loads a file into an Array^
	Platform::Array<byte>^ LoadShaderFile(std::string File);

	// Default Input Element Descriptor 
	D3D11_INPUT_ELEMENT_DESC defaultIED[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	class Shader
	{
	private:
		std::shared_ptr<DeviceResources> deviceResources;
		std::string vsFileName;
		std::string psFileName;
		std::string gsFileName;
		std::string hsFileName;
		std::string dsFileName;

	public:
		Shader(const std::shared_ptr<DeviceResources>& deviceResources);
		Shader(const std::shared_ptr<DeviceResources>& deviceResources,
			std::string vsFile, std::string psFile,
			std::string gsFile = "", std::string hsFile = "", std::string dsFile = "");

		void SetInputDescriptor(std::initializer_list<D3D11_INPUT_ELEMENT_DESC> l);
		void SetInputDescriptor(D3D11_INPUT_ELEMENT_DESC pElemDesc[], int numDesc);

		void Initialize();
		void UnInitialize();
		void Apply();
		void Reset();

		ComPtr<ID3D11InputLayout> inputlayout;			// describes semantics of vertex attributes : possibly different per shader

		ComPtr<ID3D11Buffer> constantbuffer;			// one of the constant buffers : each shader will have its own set 

		ComPtr<ID3D11VertexShader> vertexshader;		// vertex shader reference
		ComPtr<ID3D11PixelShader> pixelshader;			// pixel shader reference
		ComPtr<ID3D11GeometryShader> geometryshader;
		ComPtr<ID3D11HullShader> hullshader;
		ComPtr<ID3D11DomainShader> domainshader;

		D3D11_INPUT_ELEMENT_DESC *pIED;
		int numDescriptors;
	};
}