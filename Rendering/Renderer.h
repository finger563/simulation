/*
 *  Renderer Class 
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "d3dApp.h"
#include "RenderStates.h"
#include "Controller.h"

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

	ID3D11ShaderResourceView* mEarthDiffuseMapSRV;
	ID3D11ShaderResourceView* mEarthNormalTexSRV;
	
	ID3D11ShaderResourceView* mCloudsDiffuseMapSRV;
	ID3D11ShaderResourceView* mCloudsNormalTexSRV;

	DirectionalLight mDirLights[3];
	Material mEarthMat;

	XMFLOAT4X4 mTexTransform;
	XMFLOAT4X4 mEarthWorld;

	XMFLOAT4X4 mProj;

	int mEarthVertexOffset;
	UINT mEarthIndexOffset;
	UINT mEarthIndexCount;
	
	int mSkyVertexOffset;
	UINT mSkyIndexOffset;
	UINT mSkyIndexCount;
	
	int mCloudsVertexOffset;
	UINT mCloudsIndexOffset;
	UINT mCloudsIndexCount;
	
	int mOceanVertexOffset;
	UINT mOceanIndexOffset;
	UINT mOceanIndexCount;
	
	Controller control;
};

#endif