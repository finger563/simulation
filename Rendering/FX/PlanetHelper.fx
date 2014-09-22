
// The number of sample points taken along the ray
const int nSamples = 5;
const float fSamples = 5.0;

// The scale depth (the altitude at which the average atmospheric density is found)
const float fScaleDepth = 0.25;
const float fInvScaleDepth = 4.0;

struct VertexIn
{
	float3 PosL     : POSITION0;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
	float3 TangentL : TANGENT;

	float3 Geodetic	: POSITION1;
};

cbuffer cbPerPlanetFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;
	float3 gPlanetPosW;
	
	float3 v3CameraPos;		// The camera's current position relative to center of planet
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

	float g;
	float g2;
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

static const float PI = 3.14159265358f;

float3 surfaceTangent( float3 normal )
{
	float4 n = float4( normal.x, 0, normal.z , 1.0 );
	float angle = 90.0f;
	float fCosAngle = cos(angle);
	float fSinAngle = sin(angle);
	float4x4 R;// = XMMatrixRotationY( 90.0f );
	
    R[0][0] = fCosAngle;
    R[0][1] = 0.0f;
    R[0][2] = -fSinAngle;
    R[0][3] = 0.0f;

    R[1][0] = 0.0f;
    R[1][1] = 1.0f;
    R[1][2] = 0.0f;
    R[1][3] = 0.0f;

    R[2][0] = fSinAngle;
    R[2][1] = 0.0f;
    R[2][2] = fCosAngle;
    R[2][3] = 0.0f;

    R[3][0] = 0.0f;
    R[3][1] = 0.0f;
    R[3][2] = 0.0f;
    R[3][3] = 1.0f;

	float3 result = mul(n,R).xyz;
	return normalize(result);
}

float2 geoToTex( float3 geo )
{
	return float2( 
		geo.x / ( 2.0 * PI ),
		1.0 - (geo.y / ( PI ) + 0.5)
		);
}

float3 geoToNormal( float3 geo )
{
	float cosLat = cos( geo.y );
	float3 n = float3( 
		cosLat * cos(geo.x),
		sin(geo.y), 
		cosLat * sin(geo.x) );
	return normalize(n);
}

static const float3 radius2 = float3( 6378.140f*6378.140f, 6356.752f* 6356.752f , 6378.140f*6378.140f );

float3 geoToSurface( float3 geo )
{
	float3 n = geoToNormal( geo );
	float3 k = float3( 
		radius2.x * n.x,
		radius2.y * n.y,
		radius2.z * n.z );
	float gamma = sqrt( k.x * n.x + 
						k.y * n.y + 
						k.z * n.z );
	return float3( k.x / gamma + geo.z * n.x,
					 k.y / gamma + geo.z * n.y,
					 k.z / gamma + geo.z * n.z );
}

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
 