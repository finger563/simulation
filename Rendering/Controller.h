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
	Camera& get_Camera();
	float get_earthRadius();
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
	Camera mCam;

	POINT mLastMousePos;

	float earthRadius;	
	float maxRadius;
	float farClipPlaneDist;
};

#endif