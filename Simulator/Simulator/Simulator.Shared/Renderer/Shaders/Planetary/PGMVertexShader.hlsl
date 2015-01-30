#include "./Pipeline.hlsli"
#include "./ConstantBuffers.hlsli"

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.pos = mul(matWVP,vin.pos);
	return vout;
}