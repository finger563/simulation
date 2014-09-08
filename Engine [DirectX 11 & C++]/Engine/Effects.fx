struct Light
{
	float3 dir;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame
{
	Light light;
};

/*
 * This is the constant buffer. We create a constant buffer structure using cbuffer. 
 * Remember to separate them and name them on the frequency in which they are updated. 
 * A matrix variable in an effect file is represented by the float4x4 type. 
 * You can have other sizes too by changing the numbers.
 */
cbuffer cbPerObject
{
	float4x4 WVP;
    float4x4 World;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
    VS_OUTPUT output;

	// Translate the object's vertices according to the WVP matrix we last updated.
    output.Pos = mul(inPos, WVP);
	output.normal = mul(normal, World);
    output.TexCoord = inTexCoord;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	input.normal = normalize(input.normal);

    float4 diffuse = ObjTexture.Sample( ObjSamplerState, input.TexCoord );

	float3 finalColor;

	finalColor = diffuse * light.ambient;
	finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);
	
	return float4(finalColor, diffuse.a);
}

float4 D2D_PS(VS_OUTPUT input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
	
	return diffuse;
}
