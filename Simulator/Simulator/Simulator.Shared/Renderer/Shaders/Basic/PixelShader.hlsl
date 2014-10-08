
#include "../Common/Pipeline.hlsli"
#include "../Common/ConstantBuffers.hlsli"

float4 main( VertexOut pin ) : SV_TARGET
{
	return pin.color;
}