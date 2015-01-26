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
		element.pos = input[i];
		element.normal = float4(0, 1, 0, 0);
		output.Append(element);
	}
}

GeometryShader gsStreamOut = ConstructGSWithSO(
	CompileShader(gs_5_0, main()),
	"SV_POSITION.xyzw; NORMAL.xyzw"
	);