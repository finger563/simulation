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
TextureCube SkyMap;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct SKYMAP_VS_OUTPUT	//output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
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

SKYMAP_VS_OUTPUT SKYMAP_VS(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;

	//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.Pos = mul(float4(inPos, 1.0f), WVP).xyww;

	output.texCoord = inPos;

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

float4 SKYMAP_PS(SKYMAP_VS_OUTPUT input) : SV_Target
{
	return SkyMap.Sample(ObjSamplerState, input.texCoord);
}

float4 D2D_PS(VS_OUTPUT input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
	
	return diffuse;
}




