/*
 *  Renderer Class 
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "d3dApp.h"
#include "RenderStates.h"
#include "Controller.h"

enum RenderOptions
{
	RenderOptionsBasic = 0,
	RenderOptionsNormalMap = 1,
	RenderOptionsDisplacementMap = 2
};

class Renderer : public D3DApp
{
public:

	/*
	 * Renderer Constructor. 
	 */
	Renderer(HINSTANCE hInstance);

	/*
	 * Renderer Destructor.
	 */
	~Renderer();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene(); 

private:
	void BuildGeometryBuffers();

private:
	ID3D11Buffer* mVB;
	ID3D11Buffer* mIB;

	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11ShaderResourceView* mEarthNormalTexSRV;

	DirectionalLight mDirLights[3];
	Material mEarthMat;

	XMFLOAT4X4 mTexTransform;
	XMFLOAT4X4 mEarthWorld;
	XMFLOAT4X4 mSkyWorld;

	XMFLOAT4X4 mProj;

	int mEarthVertexOffset;
	UINT mEarthIndexOffset;
	UINT mEarthIndexCount;
	
	int mSkyVertexOffset;
	UINT mSkyIndexOffset;
	UINT mSkyIndexCount;

	RenderOptions mRenderOptions;

	Controller control;
};

#endif