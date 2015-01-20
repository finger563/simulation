
cbuffer ConstantBuffer
{
	float4x4 matWVP;
	float4x4 rotation;
	float4 lightvec;		// the diffuse light's vector
	float4 lightcol;		// the diffuse light's color
	float4 ambientcol;		// the ambient light's color
	float4 cameraPos;		// the camera's position
	float4 viewVec;			// the camera's view vector
	float4x4 texTransform;	// convert P_ij.{x,y} into texture lookups
};

// non-numeric values cannot be added to a constant buffer
Texture2D gDiffuseMap;
Texture2D gNormalMap;

// Sampler State for sampling the textures
SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};