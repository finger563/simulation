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
		//   -- should probably try to get this done in tessellator stage if possible
		// need interface here to get/apply shaders
		// should this class handle the deferred rendering

		virtual void OnSuspending();
		virtual void OnResuming();

	private:
		Camera ViewCamera;
		Camera SamplingCamera;	// same position as view camera, but different orientation and frustum
				
		// Grid Point related data
		int numGridPointsX;
		int numGridPointsY;

		// Sphere related data
		Vector sphereWorldPos;		// world space position of all spheres
		double primaryRadius;		// radius of primary sphere
		double secondaryRadius;		// radius of secondary referece sphere
		double minRadius;			// minimum radius (based on dataset)
		double maxRadius;			// maximum radius (based on dataset)

		// Shader related data: shaders perform PGM on GPU
		std::unique_ptr<Shader> pgmShader;					// raytraces grid points, produces sphere normals and positions
		std::unique_ptr<Shader> deferredTexturingShader;	// rasterizes based on datasets, sphere normals, and positions
		
		std::shared_ptr<DeviceResources> deviceResources;
	};
}