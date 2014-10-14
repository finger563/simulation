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

	class Shader
	{
	private:
		std::shared_ptr<DeviceResources> deviceResources;

		// Everything below here is now contained within device resources
		//ComPtr<ID3D11Device2> dev;						// pointer to renderer's device handle
		//ComPtr<ID3D11DeviceContext2> devcon;			// pointer to renderer's device context handle
	public:
		Shader(const std::shared_ptr<DeviceResources>& deviceResources);
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
	};
}