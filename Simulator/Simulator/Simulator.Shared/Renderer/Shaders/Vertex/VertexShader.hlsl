
cbuffer ConstantBuffer
{
	float4x4 matWVP;
	float4x4 rotation;
	float4 lightvec;     // the diffuse light's vector
	float4 lightcol;      // the diffuse light's color
	float4 ambientcol;    // the ambient light's color
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VertexOut main(float4 pos : POSITION, float4 normal : NORMAL)
{
	VertexOut vertOut;
	vertOut.position = mul(matWVP, pos);

	float4 norm = normalize(mul(rotation, normal));
	float diffusebrightness = saturate(dot(norm, lightvec));

	vertOut.color = ambientcol;
	vertOut.color += lightcol * diffusebrightness;    // find the diffuse color and add

	return vertOut;
}