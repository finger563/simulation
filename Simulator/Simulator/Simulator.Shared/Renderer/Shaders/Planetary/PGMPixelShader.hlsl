#include "./Pipeline.hlsli"
#include "./ConstantBuffers.hlsli"

float4 main(GSOutput pin) : SV_TARGET
{
	// use camera position and surface/position (plane or sphere) to project
	// pin.position to actual position
	//float4 pos = cameraPos + cameraPos.z / (cameraPos.z - pin.pos.z) * (pin.pos - cameraPos); // implements P_ij = V + (v_z)/(v_z - g_z)*(G_ij - V)
	float4 pos = pin.pos;
	
	// then calculate height/texture lookup based on pin.position
	float4 texCoord = float4(pos.xy, 0, 0);
	texCoord = mul(texCoord,texTransform);
	
	// then displace pin.position based on height lookup
	const float MipInterval = 500.0f;
	float mipLevel = clamp((distance(pos, cameraPos) - MipInterval) / MipInterval, 0.0f, 6.0f);
	
	// Sample height map
	//float h = gNormalMap.SampleLevel(samLinear, texCoord, mipLevel).a;
	//pin.pos.z += h;
	
	// then color output based on texture lookup
	float4 norm = float4(gNormalMap.SampleLevel(samLinear, texCoord, mipLevel).rgb,0);
	float diffusebrightness = saturate(dot(norm, lightvec));

	float4 color = ambientcol;
	color += lightcol * diffusebrightness;    // find the diffuse color and add
	color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return color;
}