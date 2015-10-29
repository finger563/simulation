#include "../Planetary/Pipeline.hlsli"
#include "../Common/ConstantBuffers.hlsli"

VertexOut main(VertexIn vin)
{
	VertexOut vout = vin;
	vout.pos = mul(vin.pos, matWVP);
	return vout;
}