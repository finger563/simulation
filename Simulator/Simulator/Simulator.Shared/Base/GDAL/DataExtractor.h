#pragma once

#include "Base\Subsystem.h"

namespace Base
{
	class DataExtractor : public Base::ISubsystem
	{
	public:
		
		virtual bool Initialize();	// need initialization data (number of grid points etc)
		virtual void Update();		// update camera data here
		virtual bool UnInitialize();

		// Need interface to get projection information
		// Need interface to get height data (as array)
		// Need interface to get coordinates of dataset
		//   - world coords
		//   - geographic coords
		//   - projection coords

		// Need to make sure files are left in a stable state when resuming/suspending
		virtual void OnSuspending();
		virtual void OnResuming();

	private:
		// need pointers to loaded datasets from GDAL library
	};
}