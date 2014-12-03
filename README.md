simulation
==========

This project has the goal of developing a modular, fast, and realistic multi-domain system simulation and renderer.  It is the culmination of graphics, rendering, and physics research that I've done over the past 6-7 years as either hobby or class projects.

We are developing a GPU-accelerated muti-domain physics simulation and realistic rendering system.  It will support the simulation of multiple physical domains simultaneously, e.g. fluid dynamics, gravity, elastic and inelastic collision.  Additionally, it will support realistic rendering of the simulation as feedback and interaction with the user.  We are integrating support for procedural data generation as well as the data stored by NASA in its [Planetary Data System](http://pds.jpl.nasa.gov/).  By working with existing high-fidelity physics simulation engines and integrating the latest in GPU acceleration and rendering techniques, we will provide real-time simulation and feedback to the user.

Research:
---------
Much of the research for this engine has not been documented (sadly), as it has been ongoing since around 2008.  Some of that resarch culminated in a class project which was a [networked first person shooter built on top of a custom software rendering engine](https://github.com/finger563/CS283Project).  However, Since this project has started, all resarch has been (as much as possible) catalogued by topic as [book references](BookReferences.md), [web references](WebReferences.md), or the relevant research papers have been committed into the repository under the Docs folder.  Additionally, any ongoing or past implementation notes are committed as their own files, listed under [Relevant Documentation](#relevant-documentation) in this file.

Design:
-------
A full design can be found in our [Design Document](Design.md); this section is a shorter version of it.  

The simulator is composed as a collection of subsystems which each export at least this basic interface:

	interface ISubsystem
	{
		virtual bool Initialize() = 0;
		virtual void Update() = 0;
		virtual bool UnInitialize() = 0;

		virtual void OnSuspending() = 0;
		virtual void OnResuming() = 0;
	};

The main subsystems of the simulator are
* Base
  * Contains the basic data structures, math, memory, time, and interface used across subsystems
* Engine
  * Controls the main execution of the simulator and its subsystems
* Input
  * Controls the user input to the simulator
* Physics
  * Manages the physical simulation for all entities being simulated
* Renderer
  * Manages the rendering of all entities for feedback to the user

Each of these subsystems may extend the base subsystem interface to add relevant methods required for their purpose.  

Implementation:
---------------
Currently there are two versions of the simulator implemented:
* [The Renderer Project](Rendering/Renderer.sln) in the Rendering folder is the outdated code which utilizes hardware tesselation and programmable shaders to render the NASA Earth SRTM dataset with a cloud layer and atmosphere.
* [The Simulator Project](Simulator/Simulator.sln) in the Simulator folder contains the main implementation.  It is currently being transitioned from a ROAM-based rendering implementation to a PGM-based implementation.  The user input subsystem is functional, as are the Renderer and the Engine subsystems, but the Physics subsystem has no integration with 3rd party physics simulators and contains no physics simulation code itself.  

Relevant Documentation:
-----------------------
* [The current Design Document](Design.md)
* [A list of book chapters relevant to each subject](BookReferences.md)
* [A list of websites relevant to each subject](WebReferences.md)
* [A development document full of notes for implementing and extending PGM](PGMImplementation.md)
* [A development document with notes for the possible integration of GDAL](GDALIntegration.md)
