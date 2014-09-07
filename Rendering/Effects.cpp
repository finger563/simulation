//***************************************************************************************
// Effects.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "Effects.h"

#pragma region Effect
Effect::Effect(ID3D11Device* device, const std::wstring& filename)
	: mFX(0)
{
	std::ifstream fin(filename.c_str(), std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
		0, device, &mFX));
}

Effect::~Effect()
{
	ReleaseCOM(mFX);
}
#pragma endregion

#pragma region PlanetEffect
PlanetEffect::PlanetEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	EyePosW				= mFX->GetVariableByName("gEyePosW")->AsVector();
	PlanetPosW			= mFX->GetVariableByName("gPlanetPosW")->AsVector();
	
	CameraPos				= mFX->GetVariableByName("v3CameraPos")->AsVector();
	LightPos				= mFX->GetVariableByName("v3LightPos")->AsVector();
	InvWaveLength			= mFX->GetVariableByName("v3InvWavelength")->AsVector();
	CameraHeight			= mFX->GetVariableByName("fCameraHeight")->AsScalar();
	CameraHeight2			= mFX->GetVariableByName("fCameraHeight2")->AsScalar();
	OuterRadius				= mFX->GetVariableByName("fOuterRadius")->AsScalar();
	OuterRadius2			= mFX->GetVariableByName("fOuterRadius2")->AsScalar();
	InnerRadius				= mFX->GetVariableByName("fInnerRadius")->AsScalar();
	InnerRadius2			= mFX->GetVariableByName("fInnerRadius2")->AsScalar();
	KrESun					= mFX->GetVariableByName("fKrESun")->AsScalar();
	KmESun					= mFX->GetVariableByName("fKmESun")->AsScalar();
	Kr4PI					= mFX->GetVariableByName("fKr4PI")->AsScalar();
	Km4PI					= mFX->GetVariableByName("fKm4PI")->AsScalar();
	Scale					= mFX->GetVariableByName("fScale")->AsScalar();
	ScaleOverScaleDepth		= mFX->GetVariableByName("fScaleOverScaleDepth")->AsScalar();
	G						= mFX->GetVariableByName("g")->AsScalar();
	G2						= mFX->GetVariableByName("g2")->AsScalar();
}

PlanetEffect::~PlanetEffect()
{
}
#pragma endregion

#pragma region DisplacementMapEffect
DisplacementMapEffect::DisplacementMapEffect(ID3D11Device* device, const std::wstring& filename)
	: PlanetEffect(device, filename)
{
	PlanetFromSpaceTech = mFX->GetTechniqueByName("PlanetFromSpace");
	PlanetFromAtmoTech = mFX->GetTechniqueByName("PlanetFromAtmo");

	ViewProj          = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	DirLights         = mFX->GetVariableByName("gDirLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	HeightScale       = mFX->GetVariableByName("gHeightScale")->AsScalar();
	MaxTessDistance   = mFX->GetVariableByName("gMaxTessDistance")->AsScalar();
	MinTessDistance   = mFX->GetVariableByName("gMinTessDistance")->AsScalar();
	MinTessFactor     = mFX->GetVariableByName("gMinTessFactor")->AsScalar();
	MaxTessFactor     = mFX->GetVariableByName("gMaxTessFactor")->AsScalar();
	DiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	CubeMap           = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
	NormalMap         = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
}

DisplacementMapEffect::~DisplacementMapEffect()
{
}
#pragma endregion

#pragma region OceanEffect
OceanEffect::OceanEffect(ID3D11Device* device, const std::wstring& filename)
	: PlanetEffect(device, filename)
{
	OceanFromSpaceTech = mFX->GetTechniqueByName("OceanFromSpace");
	OceanFromAtmoTech = mFX->GetTechniqueByName("OceanFromAtmo");

	ViewProj          = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	DirLights         = mFX->GetVariableByName("gDirLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	HeightScale       = mFX->GetVariableByName("gHeightScale")->AsScalar();
	MaxTessDistance   = mFX->GetVariableByName("gMaxTessDistance")->AsScalar();
	MinTessDistance   = mFX->GetVariableByName("gMinTessDistance")->AsScalar();
	MinTessFactor     = mFX->GetVariableByName("gMinTessFactor")->AsScalar();
	MaxTessFactor     = mFX->GetVariableByName("gMaxTessFactor")->AsScalar();
	DiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	CubeMap           = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
	NormalMap         = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
}

OceanEffect::~OceanEffect()
{
}
#pragma endregion

#pragma region CloudsEffect
CloudsEffect::CloudsEffect(ID3D11Device* device, const std::wstring& filename)
	: PlanetEffect(device, filename)
{
	CloudsFromSpaceTech = mFX->GetTechniqueByName("CloudsFromSpace");
	CloudsFromAtmoTech = mFX->GetTechniqueByName("CloudsFromAtmo");

	ViewProj          = mFX->GetVariableByName("gViewProj")->AsMatrix();
	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform      = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	DirLights         = mFX->GetVariableByName("gDirLights");
	Mat               = mFX->GetVariableByName("gMaterial");
	HeightScale       = mFX->GetVariableByName("gHeightScale")->AsScalar();
	MaxTessDistance   = mFX->GetVariableByName("gMaxTessDistance")->AsScalar();
	MinTessDistance   = mFX->GetVariableByName("gMinTessDistance")->AsScalar();
	MinTessFactor     = mFX->GetVariableByName("gMinTessFactor")->AsScalar();
	MaxTessFactor     = mFX->GetVariableByName("gMaxTessFactor")->AsScalar();
	DiffuseMap        = mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	CubeMap           = mFX->GetVariableByName("gCubeMap")->AsShaderResource();
	NormalMap         = mFX->GetVariableByName("gNormalMap")->AsShaderResource();
}

CloudsEffect::~CloudsEffect()
{
}
#pragma endregion

#pragma region SkyEffect
SkyEffect::SkyEffect(ID3D11Device* device, const std::wstring& filename)
	: PlanetEffect(device, filename)
{
	SkyFromSpaceTech       = mFX->GetTechniqueByName("SkyFromSpace");
	SkyFromAtmoTech       = mFX->GetTechniqueByName("SkyFromAtmo");

	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	WorldViewProj     = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World             = mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	DirLights         = mFX->GetVariableByName("gDirLights");
	Mat               = mFX->GetVariableByName("gMaterial");
}

SkyEffect::~SkyEffect()
{
}
#pragma endregion

#pragma region Effects

OceanEffect*		   Effects::OceanFX			  = 0;
CloudsEffect*		   Effects::CloudsFX		  = 0;
DisplacementMapEffect* Effects::DisplacementMapFX = 0;
SkyEffect*             Effects::SkyFX             = 0;

void Effects::InitAll(ID3D11Device* device)
{
	OceanFX			  = new OceanEffect(device, L"FX/Ocean.fxo");
	DisplacementMapFX = new DisplacementMapEffect(device, L"FX/DisplacementMap.fxo");
	CloudsFX		  = new CloudsEffect(device, L"FX/Clouds.fxo");
	SkyFX             = new SkyEffect(device, L"FX/Sky.fxo");
}

void Effects::DestroyAll()
{
	SafeDelete(OceanFX);
	SafeDelete(DisplacementMapFX);
	SafeDelete(CloudsFX);
	SafeDelete(SkyFX);
}

#pragma endregion