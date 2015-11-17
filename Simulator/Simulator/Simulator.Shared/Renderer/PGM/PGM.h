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
		struct GridVertex {
			float x, y, z, w;
		};
		struct SOVertex {
			float x, y, z, w;
			float nx, ny, nz, nw;
		};

		PGM(const std::shared_ptr<DeviceResources>& deviceResources);

		virtual bool Initialize();	// need initialization data (number of grid points etc)
		virtual void CreateWindowSizeDependentResources();
		virtual void Update();		// update camera data here
		void Render();
		virtual bool UnInitialize();

		// need interface here to update the camera
		virtual void SetViewCamera(const Renderer::Camera& c);
		virtual Renderer::Camera GetViewCamera();
		virtual void SetSamplingCamera(const Renderer::Camera& c);
		virtual Renderer::Camera GetSamplingCamera();
		// need interface here to get the grid points for rendering
		//   -- should probably try to get this done in tessellator stage if possible
		virtual void MakeGridPoints();
		virtual void SetGridPointXSize(int xsize);
		virtual void SetGridPointYSize(int ysize);
		virtual int GetGridPointXSize();
		virtual int GetGridPointYSize();
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
		ComPtr<ID3D11Buffer> gridvertexbuffer;				// Dx11.2 GPU vertex buffer interface
		ComPtr<ID3D11Buffer> gridindexbuffer;				// Dx11.2 GPU index buffer interface
		ComPtr<ID3D11Buffer> streamOutVertexBuffer;			// Dx11.2 GPU Stream-Out vertex buffer interface
		int numIndices;									// number of indices to draw

		// Sphere related data
		Vector sphereWorldPos;		// world space position of all spheres
		double primaryRadius;		// radius of primary sphere
		double secondaryRadius;		// radius of secondary referece sphere
		double minRadius;			// minimum radius (based on dataset)
		double maxRadius;			// maximum radius (based on dataset)

		// Shader related data: shaders perform PGM on GPU
		std::unique_ptr<Shader> pgmShader;					// raytraces grid points, produces sphere normals and positions
		std::unique_ptr<Shader> rasterizationShader;		// rasterizes everything else
		std::unique_ptr<Shader> deferredTexturingShader;	// rasterizes based on datasets, sphere normals, and positions
		
		std::shared_ptr<DeviceResources> deviceResources;
	};
}