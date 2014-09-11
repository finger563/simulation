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

static const float TessFactor = 2.0f;

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



//***************************************************************************************
// SKYBOX
//***************************************************************************************
TextureCube SkyMap;

struct SKYMAP_VS_OUTPUT	//output structure for skymap vertex shader
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
	float3 normal : NORMAL;
};

SKYMAP_VS_OUTPUT SKYMAP_VS(float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;

	//Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
	output.Pos = mul(float4(inPos, 1.0f), WVP).xyww;

	output.texCoord = inPos;

	return output;
}

float4 SKYMAP_PS(SKYMAP_VS_OUTPUT input) : SV_Target
{
	return SkyMap.Sample(ObjSamplerState, input.texCoord);
}

technique11 Skybox
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, SKYMAP_VS() ) );
		SetPixelShader( CompileShader( ps_5_0, SKYMAP_PS() ) );
	}
}

//*****************************************************
// TESSELLATION
//*****************************************************


struct PatchTess
{
	float EdgeTess[3] : SV_TessFactor;
	float InsideTess  : SV_InsideTessFactor;
};

PatchTess PatchHS(InputPatch<SKYMAP_VS_OUTPUT,3> patch, 
                  uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	
	// Average tess factors along edges, and pick an edge tess factor for 
	// the interior tessellation.  It is important to do the tess factor
	// calculation based on the edge properties so that edges shared by 
	// more than one triangle will have the same tessellation factor.  
	// Otherwise, gaps can appear.
	pt.EdgeTess[0] = TessFactor;
	pt.EdgeTess[1] = TessFactor;
	pt.EdgeTess[2] = TessFactor;
	pt.InsideTess  = pt.EdgeTess[0];
	
	return pt;
}

struct HullOut
{
	float3 Pos     : POSITION;
    float3 normal  : NORMAL;
	float2 texCoord      : TEXCOORD;
};

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchHS")]
HullOut TESS_HS(InputPatch<SKYMAP_VS_OUTPUT,3> p, 
           uint i : SV_OutputControlPointID,
           uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	
	// Pass through shader.
	hout.Pos     = p[i].Pos;
	hout.normal  = p[i].normal;
	hout.texCoord      = p[i].texCoord;
	return hout;
}

struct DomainOut
{
	//float4 PosH     : SV_POSITION;
    float3 PosW     : POSITION;
    float3 NormalW  : NORMAL;
	float2 Tex      : TEXCOORD;
};

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("tri")]
DomainOut TESS_DS(PatchTess patchTess, 
             float3 bary : SV_DomainLocation, 
             const OutputPatch<HullOut,3> tri)
{
	DomainOut dout;
	
	// Interpolate patch attributes to generated vertices.
	dout.PosW     = bary.x*tri[0].Pos     + bary.y*tri[1].Pos     + bary.z*tri[2].Pos;
	dout.NormalW  = bary.x*tri[0].normal  + bary.y*tri[1].normal  + bary.z*tri[2].normal;
	dout.Tex      = bary.x*tri[0].texCoord      + bary.y*tri[1].texCoord      + bary.z*tri[2].texCoord;
	
	// Interpolating normal can unnormalize it, so normalize it.
	dout.NormalW = normalize(dout.NormalW);
				
	// Project to homogeneous clip space.
	//dout.PosH = mul(float4(dout.PosW, 1.0f), WVP);
	
	return dout;
}

float4 TESSELLATE_PS(DomainOut input) : SV_Target
{
	return float4(1,1,1,1);
}

technique11 Tessellation
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, SKYMAP_VS() ) );
        SetHullShader( CompileShader( hs_5_0, TESS_HS() ) );
        SetDomainShader( CompileShader( ds_5_0, TESS_DS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, TESSELLATE_PS() ) );
	}
}
