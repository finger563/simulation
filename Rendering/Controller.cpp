#include "MathHelper.h"
#include "Renderer.h"

Controller::Controller(HINSTANCE hInstance) : D3DApp(hInstance) {}

bool Controller::Init() 
{
	earthRadius = 6500.0f;
	earthPosW = XMFLOAT3(0,0,earthRadius);
	maxRadius = earthRadius*100.0f;
	farClipPlaneDist = earthRadius*1000.0f;

	speed = 100.0f;
	minSpeed = 1.0f;
	maxSpeed = 100000.0f;

	mCam.SetPosition(0,0,-earthRadius*1.1);

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;
	return true;
}
void Controller::OnMouseWheel(WPARAM wheelState, int delta)
{
	speed += speed/(delta/10.0f);
	speed = MathHelper::Clamp(speed, minSpeed, maxSpeed);
}

void Controller::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(mhMainWnd);
}

void Controller::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}

void Controller::OnMouseMove(WPARAM btnState, int x, int y)
{
	if( (btnState & MK_LBUTTON) != 0 )
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		mCam.Pitch(dy);
		mCam.RotateY(dx);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void Controller::UpdateScene(float dt) {}

void Controller::DrawScene() {}

Camera &Controller::get_Camera()
{
	return mCam;
}

float Controller::get_Speed() 
{
	return speed;
}

float Controller::get_earthRadius() 
{
	return earthRadius;
}

XMFLOAT3 Controller::get_earthPosW() 
{
	return earthPosW;
}

float Controller::get_maxRadius() 
{
	return maxRadius;
}

float Controller::get_farClipPlaneDist() 
{
	return farClipPlaneDist;
}

POINT Controller::get_mLastMousePos() 
{
	return mLastMousePos;
}