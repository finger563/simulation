
#include "../Common/Pipeline.hlsli"
#include "../Common/ConstantBuffers.hlsli"

float4 main( VertexOut pin ) : SV_TARGET
{
	float4 norm = normalize(pin.normal);
	float diffusebrightness = saturate(dot(norm, lightvec));

	float4 color = ambientcol;
	color += lightcol * diffusebrightness;    // find the diffuse color and add
	return color;
}