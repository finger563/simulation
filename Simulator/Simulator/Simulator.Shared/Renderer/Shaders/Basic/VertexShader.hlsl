
#include "../Common/Pipeline.hlsli"
#include "../Common/ConstantBuffers.hlsli"

VertexOut main( VertexIn vin )
{
	VertexOut vout;
	vout.position = mul(matWVP, vin.pos);

	float4 norm = normalize(mul(rotation, vin.normal));
	float diffusebrightness = saturate(dot(norm, lightvec));

	vout.color = ambientcol;
	vout.color += lightcol * diffusebrightness;    // find the diffuse color and add

	return vout;
}