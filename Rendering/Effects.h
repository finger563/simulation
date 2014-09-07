//***************************************************************************************
// Effects.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines lightweight effect wrappers to group an effect and its variables.
// Also defines a static Effects class from which we can access all of our effects.
//***************************************************************************************

#ifndef EFFECTS_H
#define EFFECTS_H

#include "d3dUtil.h"

#pragma region Effect
class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion

#pragma region PlanetEffect
class PlanetEffect : public Effect
{
public:
	PlanetEffect(ID3D11Device* device, const std::wstring& filename);
	~PlanetEffect();
	
	void SetEyePosW(const XMFLOAT3& v)					{ EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetPlanetPosW(const XMFLOAT3& v)					{ PlanetPosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }

	void SetCameraPos(const XMFLOAT3& v)				{ CameraPos->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetLightPos(const XMFLOAT3& v)					{ LightPos->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetInvWaveLength(const XMFLOAT3& v)			{ InvWaveLength->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetCameraHeight(float f)                       { CameraHeight->SetFloat(f); }
	void SetCameraHeight2(float f)                      { CameraHeight2->SetFloat(f); }
	void SetOuterRadius(float f)                        { OuterRadius->SetFloat(f); }
	void SetOuterRadius2(float f)                       { OuterRadius2->SetFloat(f); }
	void SetInnerRadius(float f)                        { InnerRadius->SetFloat(f); }
	void SetInnerRadius2(float f)                       { InnerRadius2->SetFloat(f); }
	void SetKrESun(float f)								{ KrESun->SetFloat(f); }
	void SetKmESun(float f)								{ KmESun->SetFloat(f); }
	void SetKr4PI(float f)								{ Kr4PI->SetFloat(f); }
	void SetKm4PI(float f)								{ Km4PI->SetFloat(f); }
	void SetScale(float f)								{ Scale->SetFloat(f); }
	void SetScaleOverScaleDepth(float f)				{ ScaleOverScaleDepth->SetFloat(f); }
	void SetG(float f)									{ G->SetFloat(f); }
	void SetG2(float f)									{ G2->SetFloat(f); }
		
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectVectorVariable* PlanetPosW;
	
	ID3DX11EffectVectorVariable* CameraPos;
	ID3DX11EffectVectorVariable* LightPos;
	ID3DX11EffectVectorVariable* InvWaveLength;
	ID3DX11EffectScalarVariable* CameraHeight;
	ID3DX11EffectScalarVariable* CameraHeight2;
	ID3DX11EffectScalarVariable* OuterRadius;
	ID3DX11EffectScalarVariable* OuterRadius2;
	ID3DX11EffectScalarVariable* InnerRadius;
	ID3DX11EffectScalarVariable* InnerRadius2;
	ID3DX11EffectScalarVariable* KrESun;
	ID3DX11EffectScalarVariable* KmESun;
	ID3DX11EffectScalarVariable* Kr4PI;
	ID3DX11EffectScalarVariable* Km4PI;
	ID3DX11EffectScalarVariable* Scale;
	ID3DX11EffectScalarVariable* ScaleOverScaleDepth;
	ID3DX11EffectScalarVariable* G;
	ID3DX11EffectScalarVariable* G2;
};
#pragma endregion

#pragma region DisplacementMapEffect
class DisplacementMapEffect : public PlanetEffect
{
public:
	DisplacementMapEffect(ID3D11Device* device, const std::wstring& filename);
	~DisplacementMapEffect();

	void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetHeightScale(float f)                        { HeightScale->SetFloat(f); }
	void SetMaxTessDistance(float f)                    { MaxTessDistance->SetFloat(f); }
	void SetMinTessDistance(float f)                    { MinTessDistance->SetFloat(f); }
	void SetMinTessFactor(float f)                      { MinTessFactor->SetFloat(f); }
	void SetMaxTessFactor(float f)                      { MaxTessFactor->SetFloat(f); }

	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }
	
	ID3DX11EffectTechnique* PlanetFromSpaceTech;
	ID3DX11EffectTechnique* PlanetFromAtmoTech;
	
	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectScalarVariable* HeightScale;
	ID3DX11EffectScalarVariable* MaxTessDistance;
	ID3DX11EffectScalarVariable* MinTessDistance;
	ID3DX11EffectScalarVariable* MinTessFactor;
	ID3DX11EffectScalarVariable* MaxTessFactor;
 
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};
#pragma endregion

#pragma region OceanEffect
class OceanEffect : public PlanetEffect
{
public:
	OceanEffect(ID3D11Device* device, const std::wstring& filename);
	~OceanEffect();

	void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetHeightScale(float f)                        { HeightScale->SetFloat(f); }
	void SetMaxTessDistance(float f)                    { MaxTessDistance->SetFloat(f); }
	void SetMinTessDistance(float f)                    { MinTessDistance->SetFloat(f); }
	void SetMinTessFactor(float f)                      { MinTessFactor->SetFloat(f); }
	void SetMaxTessFactor(float f)                      { MaxTessFactor->SetFloat(f); }
	
	void SetTime(float f)								{ Time->SetFloat(f); }
	void SetK(const XMFLOAT2& v)						{ K->SetRawValue(&v, 0, sizeof(XMFLOAT2)); }
	void SetA(float f)									{ A->SetFloat(f); }
	void SetLambda(float f)								{ Lambda->SetFloat(f); }
	void SetPI2(float f)								{ PI2->SetFloat(f); }

	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }
	
	ID3DX11EffectTechnique* OceanFromSpaceTech;
	ID3DX11EffectTechnique* OceanFromAtmoTech;
	
	ID3DX11EffectScalarVariable* Time;
	ID3DX11EffectScalarVariable* Lambda;
	ID3DX11EffectScalarVariable* A;
	ID3DX11EffectVectorVariable* K;
	ID3DX11EffectScalarVariable* PI2;
	
	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectScalarVariable* HeightScale;
	ID3DX11EffectScalarVariable* MaxTessDistance;
	ID3DX11EffectScalarVariable* MinTessDistance;
	ID3DX11EffectScalarVariable* MinTessFactor;
	ID3DX11EffectScalarVariable* MaxTessFactor;
 
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};
#pragma endregion

#pragma region CloudsEffect
class CloudsEffect : public PlanetEffect
{
public:
	CloudsEffect(ID3D11Device* device, const std::wstring& filename);
	~CloudsEffect();

	void SetViewProj(CXMMATRIX M)                       { ViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M)                   { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetHeightScale(float f)                        { HeightScale->SetFloat(f); }
	void SetMaxTessDistance(float f)                    { MaxTessDistance->SetFloat(f); }
	void SetMinTessDistance(float f)                    { MinTessDistance->SetFloat(f); }
	void SetMinTessFactor(float f)                      { MinTessFactor->SetFloat(f); }
	void SetMaxTessFactor(float f)                      { MaxTessFactor->SetFloat(f); }

	void SetDiffuseMap(ID3D11ShaderResourceView* tex)   { DiffuseMap->SetResource(tex); }
	void SetCubeMap(ID3D11ShaderResourceView* tex)      { CubeMap->SetResource(tex); }
	void SetNormalMap(ID3D11ShaderResourceView* tex)    { NormalMap->SetResource(tex); }
	
	ID3DX11EffectTechnique* CloudsFromSpaceTech;
	ID3DX11EffectTechnique* CloudsFromAtmoTech;
	
	ID3DX11EffectMatrixVariable* ViewProj;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;
	ID3DX11EffectScalarVariable* HeightScale;
	ID3DX11EffectScalarVariable* MaxTessDistance;
	ID3DX11EffectScalarVariable* MinTessDistance;
	ID3DX11EffectScalarVariable* MinTessFactor;
	ID3DX11EffectScalarVariable* MaxTessFactor;
 
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
	ID3DX11EffectShaderResourceVariable* NormalMap;
};
#pragma endregion

#pragma region SkyEffect
class SkyEffect : public PlanetEffect
{
public:
	SkyEffect(ID3D11Device* device, const std::wstring& filename);
	~SkyEffect();

	void SetWorldViewProj(CXMMATRIX M)                  { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M)                          { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M)              { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetDirLights(const DirectionalLight* lights)   { DirLights->SetRawValue(lights, 0, 3*sizeof(DirectionalLight)); }
	void SetMaterial(const Material& mat)               { Mat->SetRawValue(&mat, 0, sizeof(Material)); }

	ID3DX11EffectTechnique* SkyFromSpaceTech;
	ID3DX11EffectTechnique* SkyFromAtmoTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* Mat;
};
#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static OceanEffect* OceanFX;
	static DisplacementMapEffect* DisplacementMapFX;
	static CloudsEffect* CloudsFX;
	static SkyEffect* SkyFX;
};
#pragma endregion

#endif // EFFECTS_H