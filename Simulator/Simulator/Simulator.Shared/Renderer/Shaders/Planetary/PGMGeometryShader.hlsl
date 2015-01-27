#include "./Pipeline.hlsli"
#include "./ConstantBuffers.hlsli"

struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
};

[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< GSOutput > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GSOutput element;
		// raycast here from grid to surface
		element.pos = cameraPos + cameraPos.z / (cameraPos.z - input[i].z) * (input[i] - cameraPos); // implements P_ij = V + (v_z)/(v_z - g_z)*(G_ij - V)
		// calculate normal to surface here
		element.normal = float4(0, 1, 0, 0);
		output.Append(element);
	}
}

GeometryShader gsStreamOut = ConstructGSWithSO(
	CompileShader(gs_5_0, main()),
	"SV_POSITION.xyzw; NORMAL.xyzw"
	);