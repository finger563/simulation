#include "pch.h"
#include "PGM.h"

namespace Renderer
{
	PGM::PGM(const std::shared_ptr<DeviceResources>& devResources) :
		deviceResources(devResources)
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