
cbuffer ConstantBuffer
{
	float4x4 matWVP;
	float4x4 rotation;
	float4 lightvec;     // the diffuse light's vector
	float4 lightcol;      // the diffuse light's color
	float4 ambientcol;    // the ambient light's color
};