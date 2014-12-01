#pragma once

#include "Base\Base.h"

#include "..\Camera\Camera.h"
#include "..\Shaders\Shader.h"
#include "..\Device Resources\DeviceResources.h"

namespace Renderer
{
	class PGM : public Base::ISubsystem
	{
	public:

		PGM(const std::shared_ptr<DeviceResources>& deviceResources);

		virtual bool Initialize();	// need initialization data (number of grid points etc)
		virtual void CreateWindowSizeDependentResources();
		virtual void Update();		// update camera data here
		virtual bool UnInitialize();

		// need interface here to get the grid points for rendering
		// need interface here to get/apply shaders
		// should this class handle the deferred texturing? or what?

		virtual void OnSuspending();
		virtual void OnResuming();

	private:
		Camera SamplingCamera;
		Camera ViewCamera;

		// need something here for setting up the shaders properly (i.e. to do the raytracing etc.)
		
		std::shared_ptr<DeviceResources> deviceResources;
	};
}