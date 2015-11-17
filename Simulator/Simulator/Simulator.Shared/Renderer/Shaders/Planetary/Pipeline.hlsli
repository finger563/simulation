
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

float myRandomMagic(float x, float y, float z)
{
	// The noise function returns a value in the range -1.0f -> 1.0f

	float3 p = floor(float3(x,y,z));
	float3 f = frac(float3(x,y,z));

	f = f*f*(3.0 - 2.0*f);
	float n = p.x + p.y*57.0 + 113.0*p.z;

	return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
		lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
		lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
			lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
}

float4 dnoise3f(float x, float y, float z)
{
	int   i, j, k;
	float u, v, w;

	i = floor(x);
	u = frac(x);
	j = floor(y);
	v = frac(y);
	k = floor(z);
	w = frac(z);

	const float du = 30.0f*u*u*(u*(u - 2.0f) + 1.0f);
	const float dv = 30.0f*v*v*(v*(v - 2.0f) + 1.0f);
	const float dw = 30.0f*w*w*(w*(w - 2.0f) + 1.0f);

	u = u*u*u*(u*(u*6.0f - 15.0f) + 10.0f);
	v = v*v*v*(v*(v*6.0f - 15.0f) + 10.0f);
	w = w*w*w*(w*(w*6.0f - 15.0f) + 10.0f);

	const float a = myRandomMagic(i + 0, j + 0, k + 0);
	const float b = myRandomMagic(i + 1, j + 0, k + 0);
	const float c = myRandomMagic(i + 0, j + 1, k + 0);
	const float d = myRandomMagic(i + 1, j + 1, k + 0);
	const float e = myRandomMagic(i + 0, j + 0, k + 1);
	const float f = myRandomMagic(i + 1, j + 0, k + 1);
	const float g = myRandomMagic(i + 0, j + 1, k + 1);
	const float h = myRandomMagic(i + 1, j + 1, k + 1);

	const float k0 = a;
	const float k1 = b - a;
	const float k2 = c - a;
	const float k3 = e - a;
	const float k4 = a - b - c + d;
	const float k5 = a - c - e + g;
	const float k6 = a - b - e + f;
	const float k7 = -a + b + c - d + e - f - g + h;

	float4 vout;
	vout.x = k0 + k1*u + k2*v + k3*w + k4*u*v + k5*v*w + k6*w*u + k7*u*v*w;
	vout.y = du * (k1 + k4*v + k6*w + k7*v*w);
	vout.z = dv * (k2 + k5*w + k4*u + k7*w*u);
	vout.w = dw * (k3 + k6*u + k5*v + k7*u*v);
	return vout;
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