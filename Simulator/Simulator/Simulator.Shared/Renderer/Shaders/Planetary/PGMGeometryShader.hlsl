#include "./Pipeline.hlsli"
#include "./ConstantBuffers.hlsli"

struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
};

[maxvertexcount(1)]
void main(
	point VertexOut input[1], 
	inout PointStream< GSOutput > output
)
{
	GSOutput element;
	// raycast here from grid to surface
	//element.pos = cameraPos + cameraPos.y / (cameraPos.y - input[0].pos.y) * (input[0].pos - cameraPos); // implements P_ij = V + (v_z)/(v_z - g_z)*(G_ij - V)
	element.pos = input[0].pos;
	// calculate normal to surface here
	element.normal = float4(0, 1, 0, 0);
	output.Append(element);
}