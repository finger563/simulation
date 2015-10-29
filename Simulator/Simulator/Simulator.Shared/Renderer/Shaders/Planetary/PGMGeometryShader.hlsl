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
	element.pos = input[0].pos + cameraPos.y / (cameraPos.y - input[0].pos.y) * (input[0].pos - cameraPos);
	
	//float4 mypos = (0, 1, -5, 1);
	//element.pos = mypos + mypos.y / (mypos.y - input[0].pos.y) * (input[0].pos - mypos);
	
	// calculate normal to surface here
	
	//		simple test just uses plane normal
	//element.normal = float4(0, 1, 0, 1);

	// Write to output
	if (element.pos.z >= 0)
		output.Append(element);
}