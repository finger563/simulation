#include "./Pipeline.hlsli"
#include "./ConstantBuffers.hlsli"

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.pos = vin.pos;
	return vout;
}