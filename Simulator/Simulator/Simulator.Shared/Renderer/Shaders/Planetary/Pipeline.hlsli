
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

struct RastVertOut
{
	float4 pos : SV_POSITION;
	float4 normal : NORMAL;
};

float hash(float n)
{
	return frac(sin(n)*43758.5453);
}

float noise(float3 x)
{
	// The noise function returns a value in the range -1.0f -> 1.0f

	float3 p = floor(x);
	float3 f = frac(x);

	f = f*f*(3.0 - 2.0*f);
	float n = p.x + p.y*57.0 + 113.0*p.z;

	return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
		lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
		lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
			lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}

double stripes(double x, double f)
{
	double t = 0.5 + 0.5 * sin(f * 2 * 3.14159265358 * x);
	return t * t - 0.5;
}

double turbulence(double x, double y, double z, double f)
{
	double t = -0.5;
	return 1.0;
}