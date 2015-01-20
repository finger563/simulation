#include "./Pipeline.hlsli"
#include "./ConstantBuffers.hlsli"

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.position = vin.pos;
	return vout;
}