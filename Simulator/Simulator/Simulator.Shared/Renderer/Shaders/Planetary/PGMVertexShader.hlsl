#include "../Planetary/Pipeline.hlsli"
#include "../Common/ConstantBuffers.hlsli"

RastVertOut main(GSOutput vin)
{
	RastVertOut vout;
	vout.normal = vin.normal;
	vout.pos = mul(matWVP, vin.pos);
	return vout;
}