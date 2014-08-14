#include "MathHelper.h"
#include "Renderer.h"

Controller::Controller(HINSTANCE hInstance) : D3DApp(hInstance) {}

bool Controller::Init() 
{
	earthRadius = 6500.0f;
	maxRadius = earthRadius*100.0f;
	farClipPlaneDist = earthRadius*1000.0f;

	mTheta = 1.3f*MathHelper::Pi;
	mPhi = 0.4f*MathHelper::Pi;
	mRadius = earthRadius*1.1f;	

	mLastMousePos.x = 0;
	mLastMousePos.y = 0;
	return true;
}
void Controller::OnMouseWheel(WPARAM wheelState, int delta)
{
	// Update the camera radius based on input.
	mRadius += (delta/100.0f)*((mRadius-earthRadius)/10.0f);

	// Restrict the radius.
	mRadius = MathHelper::Clamp(mRadius, earthRadius, maxRadius);
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

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi   += dy;

		// Restrict the angle mPhi.
		mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi-0.1f);
	}
	else if( (btnState & MK_RBUTTON) != 0 )
	{
		// Make each pixel correspond to 0.01 unit in the scene.
		float dx = 0.01f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.01f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += (dx - dy)*((mRadius-earthRadius)/10.0f);

		// Restrict the radius.
		mRadius = MathHelper::Clamp(mRadius, earthRadius, maxRadius);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void Controller::UpdateScene(float dt) {}

void Controller::DrawScene() {}

float Controller::get_earthRadius() 
{
	return earthRadius;
}

float Controller::get_maxRadius() 
{
	return maxRadius;
}

float Controller::get_farClipPlaneDist() 
{
	return farClipPlaneDist;
}

float Controller::get_mTheta() 
{
	return mTheta;
}

float Controller::get_mPhi() 
{
	return mPhi;
}


float Controller::get_mRadius() 
{
	return mRadius;
}

POINT Controller::get_mLastMousePos() 
{
	return mLastMousePos;
}