#include "MathHelper.h"
#include "Renderer.h"

Controller::Controller(HINSTANCE hInstance) : D3DApp(hInstance) {}

bool Controller::Init() 
{
	earthRadius = 6378.140f;
	skyAltitude = earthRadius * 0.025f;
	earthPosW = XMFLOAT3(0,0,-earthRadius*2.0f);
	earthAngle = 0;

	farClipPlaneDist = earthRadius*1000.0f;
	nearClipPlaneDist = 1.0f;

	speed = earthRadius;
	minSpeed = 1.0f;
	maxSpeed = 100000.0f;

	mCam.LookAt(XMFLOAT3(0,0,0),
				earthPosW,
				XMFLOAT3(0,1,0));

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
		mCam.RotateUp(dx);
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

void Controller::set_Speed(float s) 
{
	speed = s;
	speed = MathHelper::Clamp(speed, minSpeed, maxSpeed);
}

float Controller::get_earthRadius() 
{
	return earthRadius;
}

float Controller::get_skyAltitude()
{
	return skyAltitude;
}

XMFLOAT3 Controller::get_earthPosW() 
{
	return earthPosW;
}

float Controller::get_earthAngle() 
{
	return earthAngle;
}

void Controller::set_earthAngle(float a) 
{
	earthAngle = fmod(a, MathHelper::Pi * 2.0f);
}

float Controller::get_farClipPlaneDist() 
{
	return farClipPlaneDist;
}

void Controller::set_farClipPlaneDist(float f) 
{
	farClipPlaneDist = f;
}

float Controller::get_nearClipPlaneDist() 
{
	return nearClipPlaneDist;
}

void Controller::set_nearClipPlaneDist(float d) 
{
	nearClipPlaneDist = d;
}

POINT Controller::get_mLastMousePos() 
{
	return mLastMousePos;
}