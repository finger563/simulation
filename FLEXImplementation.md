This document covers the implementation details and challenges/questions for the integration of the BETA version of [NVIDIA's FLEX](https://developer.nvidia.com/physx-flex) as one of the physics simulation subsystems. FLEX is able to perform high-fidelity, real-time mutli-domain simulation of fluid dynamics, aerodynamics, elastic and inelastic collision, gravity, deformation, and phase transition effects.

Goals:
------
* Integration of FLEX into Simulation project
* Collision detection and resolution using FLEX for inelastic collisions between simulated objects (e.g. planetary gravity + object)
* Fluid simulation 
* Gas simulation
* N-body gravity

Extensions:
-----------
* Can we use the FLEX fluid physics simulation to provide more accurate aerodynamics models (w.r.t. drag/lift forces) than traditional aerodynamics sims like JSBSIM?

Questions:
----------
* How easy is it to get information about the simulated objects back?  E.g. for sensor and actuation data?

Unresolved Issues:
------------------
* Currently requires NVIDIA GPUs