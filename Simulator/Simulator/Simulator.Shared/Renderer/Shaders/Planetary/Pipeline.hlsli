
struct VertexIn
{
	float4 pos : POSITION;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
};

struct GSOutput
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
};

struct RastVertIn
{
	float4 pos : POSITION;
	float4 normal : NORMAL;
};

struct RastVertOut
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
};