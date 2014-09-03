
// The number of sample points taken along the ray
const int nSamples = 5;
const float fSamples = 5.0;

// The scale depth (the altitude at which the average atmospheric density is found)
const float fScaleDepth = 0.25;
const float fInvScaleDepth = 4.0;

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
 