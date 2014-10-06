
cbuffer ConstantBuffer
{
	float4x4 matWVP;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VertexOut main( float4 pos : POSITION, float4 color : COLOR )
{
	VertexOut vertOut;
	vertOut.position = mul(matWVP, pos);
	//vertOut.position = pos;
	vertOut.color = color;
	return vertOut;
}