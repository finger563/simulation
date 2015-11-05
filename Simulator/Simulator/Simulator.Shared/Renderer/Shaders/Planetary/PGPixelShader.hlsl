#include "../Planetary/Pipeline.hlsli"
#include "../Common/ConstantBuffers.hlsli"

float4 main(GSOutput pin) : SV_TARGET
{
	return pin.normal;// / abs(pin.pos.z - cameraPos.z);
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}