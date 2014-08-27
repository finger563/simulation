/*
 *  Mouse Scroll Class
 */

#ifndef Controller_H
#define Controller_H

#include "d3dApp.h"
#include "Camera.h"

#include <WinDef.h>
#include <WinUser.h>

class Controller : D3DApp
{

public: 

	Controller(HINSTANCE hInstance);

	bool Init();
	void UpdateScene(float dt);
	void DrawScene(); 

	/*
	 * Accessor Functions
	 */
	float get_Speed();
	Camera& get_Camera();
	float get_earthRadius();
	XMFLOAT3 get_earthPosW();
	float get_maxRadius();
	float get_farClipPlaneDist();
	POINT get_mLastMousePos();

	/*
	 *  Mouse Control
	 */
	void OnMouseWheel(WPARAM wheelState, int delta);
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

private: 
	float speed;
	float minSpeed, maxSpeed;

	Camera mCam;

	POINT mLastMousePos;

	float earthRadius;	

	XMFLOAT3 earthPosW;

	float maxRadius;
	float farClipPlaneDist;
};

#endif