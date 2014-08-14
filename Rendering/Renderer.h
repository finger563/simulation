/*
 *  Renderer Class 
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "d3dApp.h"
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
	ID3D11Buffer* mBoxVB;
	ID3D11Buffer* mBoxIB;

	ID3D11ShaderResourceView* mDiffuseMapSRV;

	DirectionalLight mDirLights[3];
	Material mBoxMat;

	XMFLOAT4X4 mTexTransform;
	XMFLOAT4X4 mBoxWorld;

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;

	int mBoxVertexOffset;
	UINT mBoxIndexOffset;
	UINT mBoxIndexCount;

	XMFLOAT3 mEyePosW;

	Controller control;
};

#endif