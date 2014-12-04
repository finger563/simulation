#pragma once

#include "Base\Subsystem.h"

namespace Base
{
	class DataCacher : public Base::ISubsystem
	{
	public:

		virtual bool Initialize();	// need initialization data (number of grid points etc)
		virtual void Update();		// update camera data here
		virtual bool UnInitialize();

		// Interfaces needed:
		//   - search for renderable data
		//   - preprocess data (generate mipmaps, BVH, error bounds, quadtree, etc.)
		//   - composite data
		//   - insert new data
		//   - update data (deformed, write to disk?)
		//   - set parameters (memory, screen error, etc.)

		// Need to make sure files are left in a stable state when resuming/suspending
		virtual void OnSuspending();
		virtual void OnResuming();

	private:
		// need member here for storing users' maximum allowable screen error
		// need data structure here for BVH
		// need data structure here for QuadTree
		// need GDAL DataExtractor object/ref here
	};
}