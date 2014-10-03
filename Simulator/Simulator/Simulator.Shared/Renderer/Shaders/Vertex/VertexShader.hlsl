
cbuffer ConstantBuffer
{
	float3 offset;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VertexOut main( float4 pos : POSITION, float4 color : COLOR )
{
	VertexOut vertOut;
	vertOut.position = pos;
	vertOut.color = color;
	vertOut.position.x += offset.x;
	vertOut.position.y += offset.y;
	vertOut.position.xy *= offset.z;
	return vertOut;
}