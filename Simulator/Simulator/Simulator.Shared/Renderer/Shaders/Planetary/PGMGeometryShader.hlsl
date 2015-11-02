#include "../Planetary/Pipeline.hlsli"
#include "../Common/ConstantBuffers.hlsli"

[maxvertexcount(3)]
void main(
	triangle VertexOut input[3], 
	inout TriangleStream< GSOutput > output
)
{
	GSOutput element[3];
	
	//if (input[0].pos.y < cameraPos.y && input[1].pos.y < cameraPos.y && input[2].pos.y < cameraPos.y)
	{
		// raycast here from grid to surface

		//		simple test just raycasts to plane
		// implements P_ij = V + (v_z)/(v_z - g_z)*(G_ij - V)
		element[0].pos = cameraPos + cameraPos.y / (cameraPos.y - input[0].pos.y) * (input[0].pos - cameraPos);
		element[1].pos = cameraPos + cameraPos.y / (cameraPos.y - input[1].pos.y) * (input[1].pos - cameraPos);
		element[2].pos = cameraPos + cameraPos.y / (cameraPos.y - input[2].pos.y) * (input[2].pos - cameraPos);

		// calculate normal to surface here

		//		simple test just uses plane normal
		//element.normal = float4(0, 1, 0, 1);

		// Write to output
		//output.Append(element);

		// Write to output
		//element[0].pos = input[0].pos;
		//element[1].pos = input[1].pos;
		//element[2].pos = input[2].pos;
		output.Append(element[0]);
		output.Append(element[1]);
		output.Append(element[2]);
		output.RestartStrip();
	}
}