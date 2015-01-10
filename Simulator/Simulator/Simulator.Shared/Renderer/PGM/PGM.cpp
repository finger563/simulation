#include "pch.h"
#include "PGM.h"

namespace Renderer
{
	PGM::PGM(const std::shared_ptr<DeviceResources>& devResources) :
		deviceResources(devResources),
		numGridPointsX(16),
		numGridPointsY(16)
	{
	}

	bool PGM::Initialize()
	{
		return true;
	}

	void PGM::CreateWindowSizeDependentResources()
	{

	}

	void PGM::Update()
	{
		// update view camera
		ViewCamera.UpdateMatrices();
		// update grid points (per the new viewport)
		// compute ray-sphere intersection point & sphere intersection normal
		//   gamma1 & gamma 2
		//		* gamma1 = asin((d / r) sin w) - w : first intersection angle from nadir
		//		* gamma2 = -asin((d / r) sin w) - w + pi : second intersection angle from nadir
		// update sampling camera

	}

	bool PGM::UnInitialize()
	{
		return true;
	}

	void PGM::OnSuspending()
	{

	}

	void PGM::OnResuming()
	{

	}
}