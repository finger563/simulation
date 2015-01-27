#include "./Pipeline.hlsli"
#include "./ConstantBuffers.hlsli"

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.position = float4(vin.pos,0);
	return vout;
}