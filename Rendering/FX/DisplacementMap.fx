//=============================================================================
// NormalMap.fx by Frank Luna (C) 2011 All Rights Reserved.
//=============================================================================

#include "LightHelper.fx"

// The number of sample points taken along the ray
const int nSamples = 5;
const float fSamples = 5.0;

// The scale depth (the altitude at which the average atmospheric density is found)
const float fScaleDepth = 0.25;
const float fInvScaleDepth = 4.0;
 
cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;
	
	float g;
	float g2;
	
	float3 v3CameraPos;		// The camera's current position
	float3 v3LightPos;		// The direction vector to the light source
	float3 v3InvWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
	float fCameraHeight;	// The camera's current height
	float fCameraHeight2;	// fCameraHeight^2
	float fOuterRadius;		// The outer (atmosphere) radius
	float fOuterRadius2;	// fOuterRadius^2
	float fInnerRadius;		// The inner (planetary) radius
	float fInnerRadius2;	// fInnerRadius^2
	float fKrESun;			// Kr * ESun
	float fKmESun;			// Km * ESun
	float fKr4PI;			// Kr * 4 * PI
	float fKm4PI;			// Km * 4 * PI
	float fScale;			// 1 / (fOuterRadius - fInnerRadius)
	float fScaleOverScaleDepth;	// fScale / fScaleDepth

	float  gFogStart;
	float  gFogRange;
	float4 gFogColor; 
	
	float gHeightScale;
	float gMaxTessDistance;
	float gMinTessDistance;
	float gMinTessFactor;
	float gMaxTessFactor;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gWorldInvTranspose;
	float4x4 gViewProj;
	float4x4 gWorldViewProj;
	float4x4 gTexTransform;
	Material gMaterial;
}; 

// Nonnumeric values cannot be added to a cbuffer.
Texture2D gDiffuseMap;
Texture2D gNormalMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// The scale equation calculated by Vernier's Graphical Analysis
float scale(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

// Calculates the Mie phase function
float getMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos2) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
}

// Calculates the Rayleigh phase function
float getRayleighPhase(float fCos2)
{
	//return 1.0;
	return 0.75 + 0.75*fCos2;
}

// Returns the near intersection point of a line and a sphere
float getNearIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
#if 1
	float A = dot(v3Ray,v3Ray);
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B - sqrt(fDet));
#else
	float3 dist = v3Pos - gPlanetPosW;
	float A = dot(v3Ray,v3Ray);
	float B = 2*dot(dist,v3Ray);
	float C = dot(dist,dist) - fRadius2;
	float fDet = max(0.0, B*B - 4.0f * C * A);
	float bac = sqrt(fDet);
	float t0 = (-B + bac)/(2*A);
	float t1 = (-B - bac)/(2*A);
	if (t0 > t1)
		return t1;
	else
		return t0;
#endif
}

// Returns the far intersection point of a line and a sphere
float getFarIntersection(float3 v3Pos, float3 v3Ray, float fDistance2, float fRadius2)
{
#if 1
	float A = dot(v3Ray,v3Ray);
	float B = 2.0 * dot(v3Pos, v3Ray);
	float C = fDistance2 - fRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	return 0.5 * (-B + sqrt(fDet));
#else	
	float3 dist = v3Pos - gPlanetPosW;
	float A = dot(v3Ray,v3Ray);
	float B = 2*dot(dist,v3Ray);
	float C = dot(dist,dist) - fRadius2;
	float fDet = max(0.0, B*B - 4.0f * C * A);
	float bac = sqrt(fDet);
	float t0 = (-B + bac)/(2*A);
	float t1 = (-B - bac)/(2*A);
	if (t0 < t1)
		return t1;
	else
		return t0;
#endif
}
 
struct VertexIn
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
	float3 TangentL : TANGENT;
};

struct VertexOut
{
    float3 PosW       : POSITION;
    float3 NormalW    : NORMAL;
	float3 TangentW   : TANGENT;
	float2 Tex        : TEXCOORD;
	float  TessFactor : TESS;
	
	float4 c0		  : COLOR0;
	float4 c1		  : COLOR1;
};

VertexOut VS_PlanetFromSpace(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW     = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW  = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	float d = distance(vout.PosW, gEyePosW);

	// Normalized tessellation factor. 
	// The tessellation is 
	//   0 if d >= gMinTessDistance and
	//   1 if d <= gMaxTessDistance.  
	float tess = saturate( (gMinTessDistance - d) / (gMinTessDistance - gMaxTessDistance) );
	
	// Rescale [0,1] --> [gMinTessFactor, gMaxTessFactor].
	vout.TessFactor = gMinTessFactor + tess*(gMaxTessFactor-gMinTessFactor);

	float3 v3Pos = vin.PosL.xyz;
	float3 v3Ray = v3Pos - v3CameraPos;
	v3Pos = normalize(v3Pos);
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	float fNear = getNearIntersection(v3CameraPos, v3Ray, fCameraHeight2, fOuterRadius2);

	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = v3CameraPos + v3Ray * fNear;
	fFar -= fNear;
	float fDepth = exp((fInnerRadius - fOuterRadius) * fInvScaleDepth);
	float fCameraAngle = dot(-v3Ray, v3Pos);
	float fLightAngle = dot(v3LightPos, v3Pos);
	float fCameraScale = scale(fCameraAngle);
	float fLightScale = scale(fLightAngle);
	float fCameraOffset = fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);

	// Initialize the scattering loop variables
	//gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	float3 v3Attenuate;
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fScatter = fDepth*fTemp - fCameraOffset;
		v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	vout.c0.rgb = v3FrontColor * (v3InvWavelength * fKrESun + fKmESun);
	vout.c1.rgb = v3Attenuate;
	vout.c1.a = 1;
	vout.c0.a = 1;

	return vout;
}

VertexOut VS_PlanetFromAtmo(VertexIn vin)
{
	VertexOut vout;
	
	// Transform to world space space.
	vout.PosW     = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.NormalW  = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);

	// Output vertex attributes for interpolation across triangle.
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

	float d = distance(vout.PosW, gEyePosW);

	// Normalized tessellation factor. 
	// The tessellation is 
	//   0 if d >= gMinTessDistance and
	//   1 if d <= gMaxTessDistance.  
	float tess = saturate( (gMinTessDistance - d) / (gMinTessDistance - gMaxTessDistance) );
	
	// Rescale [0,1] --> [gMinTessFactor, gMaxTessFactor].
	vout.TessFactor = gMinTessFactor + tess*(gMaxTessFactor-gMinTessFactor);
	
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	float3 v3Pos = vin.PosL.xyz;
	float3 v3Ray = v3Pos - v3CameraPos;
	v3Pos = normalize(v3Pos);
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = v3CameraPos;
	float fDepth = exp((fInnerRadius - fCameraHeight) * fInvScaleDepth);
	float fCameraAngle = dot(-v3Ray, v3Pos);
	float fLightAngle = dot(v3LightPos, v3Pos);
	float fCameraScale = scale(fCameraAngle);
	float fLightScale = scale(fLightAngle);
	float fCameraOffset = fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);

	// Initialize the scattering loop variables
	//gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	float3 v3Attenuate;
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fScatter = fDepth*fTemp - fCameraOffset;
		v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	vout.c0.rgb = v3FrontColor * (v3InvWavelength * fKrESun + fKmESun);
	vout.c1.rgb = v3Attenuate;
	vout.c1.a = 1;
	vout.c0.a = 1;

	return vout;
}

struct PatchTess
{
	float EdgeTess[3] : SV_TessFactor;
	float InsideTess  : SV_InsideTessFactor;
};

PatchTess PatchHS(InputPatch<VertexOut,3> patch, 
                  uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	
	// Average tess factors along edges, and pick an edge tess factor for 
	// the interior tessellation.  It is important to do the tess factor
	// calculation based on the edge properties so that edges shared by 
	// more than one triangle will have the same tessellation factor.  
	// Otherwise, gaps can appear.
	pt.EdgeTess[0] = 0.5f*(patch[1].TessFactor + patch[2].TessFactor);
	pt.EdgeTess[1] = 0.5f*(patch[2].TessFactor + patch[0].TessFactor);
	pt.EdgeTess[2] = 0.5f*(patch[0].TessFactor + patch[1].TessFactor);
	pt.InsideTess  = pt.EdgeTess[0];
	
	return pt;
}

struct HullOut
{
	float3 PosW     : POSITION;
    float3 NormalW  : NORMAL;
	float3 TangentW : TANGENT;
	float2 Tex      : TEXCOORD;
	
	float4 c0		  : COLOR0;
	float4 c1		  : COLOR1;
};

[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchHS")]
HullOut HS(InputPatch<VertexOut,3> p, 
           uint i : SV_OutputControlPointID,
           uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	
	// Pass through shader.
	hout.PosW     = p[i].PosW;
	hout.NormalW  = p[i].NormalW;
	hout.TangentW = p[i].TangentW;
	hout.Tex      = p[i].Tex;

	hout.c0		  = p[i].c0;
	hout.c1		  = p[i].c1;
	return hout;
}

struct DomainOut
{
	float4 PosH     : SV_POSITION;
    float3 PosW     : POSITION;
    float3 NormalW  : NORMAL;
	float3 TangentW : TANGENT;
	float2 Tex      : TEXCOORD;
	
	float4 c0		  : COLOR0;
	float4 c1		  : COLOR1;
};

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("tri")]
DomainOut DS(PatchTess patchTess, 
             float3 bary : SV_DomainLocation, 
             const OutputPatch<HullOut,3> tri)
{
	DomainOut dout;
	
	// Interpolate patch attributes to generated vertices.
	dout.PosW     = bary.x*tri[0].PosW     + bary.y*tri[1].PosW     + bary.z*tri[2].PosW;
	dout.NormalW  = bary.x*tri[0].NormalW  + bary.y*tri[1].NormalW  + bary.z*tri[2].NormalW;
	dout.TangentW = bary.x*tri[0].TangentW + bary.y*tri[1].TangentW + bary.z*tri[2].TangentW;
	dout.Tex      = bary.x*tri[0].Tex      + bary.y*tri[1].Tex      + bary.z*tri[2].Tex;
	
	dout.c0      = bary.x*tri[0].c0      + bary.y*tri[1].c0      + bary.z*tri[2].c0;
	dout.c1      = bary.x*tri[0].c1      + bary.y*tri[1].c1      + bary.z*tri[2].c1;
	
	// Interpolating normal can unnormalize it, so normalize it.
	dout.NormalW = normalize(dout.NormalW);
	
	//
	// Displacement mapping.
	//
	
	// Choose the mipmap level based on distance to the eye; specifically, choose
	// the next miplevel every MipInterval units, and clamp the miplevel in [0,6].
	const float MipInterval = 20.0f;
	float mipLevel = clamp( (distance(dout.PosW, gEyePosW) - MipInterval) / MipInterval, 0.0f, 6.0f);
	
	// Sample height map (stored in alpha channel).
	float h = gNormalMap.SampleLevel(samLinear, dout.Tex, mipLevel).a;
	
	// Offset vertex along normal.
	dout.PosW += (gHeightScale*h)*dout.NormalW;
	
	// Project to homogeneous clip space.
	dout.PosH = mul(float4(dout.PosW, 1.0f), gViewProj);
	
	return dout;
}
 
float4 PS(DomainOut pin, 
          uniform int gLightCount, 
		  uniform bool gUseTexure, 
		  uniform bool gAlphaClip, 
		  uniform bool gFogEnabled, 
		  uniform bool gReflectionEnabled) : SV_Target
{
	// Interpolating normal can unnormalize it, so normalize it.
	pin.NormalW = normalize(pin.NormalW);

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;
	
    // Default to multiplicative identity.
    float4 texColor = float4(1, 1, 1, 1);
    if(gUseTexure)
	{
		// Sample texture.
		texColor = gDiffuseMap.Sample( samLinear, pin.Tex );

		if(gAlphaClip)
		{
			// Discard pixel if texture alpha < 0.1.  Note that we do this
			// test as soon as possible so that we can potentially exit the shader 
			// early, thereby skipping the rest of the shader code.
			clip(texColor.a - 0.1f);
		}
	}

	//
	// Normal mapping
	//

	float3 normalMapSample = gNormalMap.Sample(samLinear, pin.Tex).rgb;
	float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
	 
	//
	// Lighting.
	//

	float4 litColor = texColor;
	if( gLightCount > 0  )
	{  
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		[unroll]
		for(int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(gMaterial, gDirLights[i], bumpedNormalW, toEye, 
				A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}

		litColor = texColor*(ambient + diffuse) + spec;
		litColor = litColor * pin.c1;
		litColor += pin.c0;

	}

	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;

    return litColor;
}

technique11 PlanetFromSpace
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_PlanetFromSpace() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(1, true, false, false, false) ) );
    }
}

technique11 PlanetFromAtmo
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_PlanetFromAtmo() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(1, true, false, false, false) ) );
    }
}