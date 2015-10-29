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
	
	//		simple test just raycasts to plane
	// implements P_ij = V + (v_z)/(v_z - g_z)*(G_ij - V)
	element.pos = cameraPos + cameraPos.y / (cameraPos.y - input[0].pos.y) * (input[0].pos - cameraPos);
	
	// calculate normal to surface here
	
	//		simple test just uses plane normal
	//element.normal = float4(0, 1, 0, 1);

	// Write to output
	output.Append(element);
}