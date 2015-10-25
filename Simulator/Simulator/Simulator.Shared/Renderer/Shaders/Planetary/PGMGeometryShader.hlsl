#include "../Planetary/Pipeline.hlsli"
#include "../Common/ConstantBuffers.hlsli"

[maxvertexcount(1)]
void main(
	point VertexOut input[1], 
	inout PointStream< GSOutput > output
)
{
	GSOutput element;
	// raycast here from grid to surface
	float4 mypos = float4(0, 1, -5, 0);
	element.pos = cameraPos + cameraPos.y / (cameraPos.y - input[0].pos.y) * (input[0].pos - cameraPos); // implements P_ij = V + (v_z)/(v_z - g_z)*(G_ij - V)
	element.pos = mypos + mypos.y / (mypos.y - input[0].pos.y) * (input[0].pos - mypos); // implements P_ij = V + (v_z)/(v_z - g_z)*(G_ij - V)
	// calculate normal to surface here
	//element.normal = float4(0, 1, 0, 0);
	output.Append(element);
}