/*
 *  Renderer Class 
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "d3dApp.h"
#include "Sky.h"
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
	Sky* mSky;

	ID3D11Buffer* mBoxVB;
	ID3D11Buffer* mBoxIB;

	ID3D11ShaderResourceView* mDiffuseMapSRV;
	ID3D11ShaderResourceView* mEarthNormalTexSRV;

	DirectionalLight mDirLights[3];
	Material mBoxMat;

	XMFLOAT4X4 mTexTransform;
	XMFLOAT4X4 mBoxWorld;

	XMFLOAT4X4 mProj;

	int mBoxVertexOffset;
	UINT mBoxIndexOffset;
	UINT mBoxIndexCount;

	RenderOptions mRenderOptions;

	Controller control;
};

#endif