
#include "../Common/Pipeline.hlsli"
#include "../Common/ConstantBuffers.hlsli"

VertexOut main( VertexIn vin )
{
	VertexOut vout;
	vout.position = mul(matWVP, vin.pos);

	float4 norm = normalize(mul(rotation, vin.normal));
	vout.normal = norm;

	return vout;
}