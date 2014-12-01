
struct VertexIn
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
	float4 color : COLOR;
};