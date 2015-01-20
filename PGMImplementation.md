This document covers the implementation details and challenges/questions for the transition from planetary mesh-based approach to a PGM-based approach. The implementation for this transition will be mainly derived from "Projective Grid Mapping for Planetary Terrain" by Joseph Mahsman.

PGM (which itself was adapted from water displacement projective mapping) is an extension of the ideas behind shadow mapping and projective texturing which project from a camera/eye point into the scene to perform depth queries, texturing, occlusion testing, etc.

The book **3D Game Programming with DirectX 11** has a chapter, _Chapter 21_, which covers projective texturing and shadow mapping.  The concepts in that chapter (core ideas, transformations, code) are all relevant to PGM.

# Things Left to Do:
--------------------
* Sampling/Auxiliary camera calculation:
  * Determine which rays should intersect the terrain; based on point underneath viewer, and extent of terrain in view camera
  * use position of view camera
  * set up frustum based on calculated rays for the four corners to define the frustum
* Grid point calculation:
  * use sampling camera frustum corner rays and generate vertices inside the sampling camera frustum (build into vertex and index lists)
* Shader ray-tracing:
  * vertex shader : 
    * pass through?
  * fragment shader : 
    * ray trace grid point from eye to surface (plane or sphere) to get position of point
    * determine normal at new point
    * determine lookup at new point (for texturing and height offset)
* Persistent Grid Mapping:
  * Use plane of y=0 as base terrain and extrude up along normal from there, similarly to how it is done for water simulation
* Sphere integration:
  * Project grid points onto sphere instead of plane
* Deferred Rendering:
  * Use multiple render targets in first pass to render fragment positions and normals, then use those as read targets in second pass for lighting and texturing

# Goals:
--------
* proper texturing of closed surfaces : resolve problem at poles and at IDL (normals)
* multiple frustum rendering
* proper LOD for planetary rendering
* multiple dataset inclusion (high-res & low-res; polar & equirectangular)

# Extensions:
-------------
* Proper dataset determination : checking the right datasets
* Streaming only the relevant chunks of the datasets (for very large datasets)
* Extension to allow planetary ocean rendering based on PGM
* Allow height determination for collision detectino
* rendering atmosphere & clouds this way?
* Generate grid points from impostors using tesselator stage
  * all objects rendered with PGM start as billboards & tesselator generates grid points based on distance to object
* Use impostors for everything
  * can we break complex objects down into connected simple objects replaced with impostors and rendered with pgm?
* How will the following work with PGM:
  * Animation
  * Non-simple bodies
  * texture warping
  * Non-convex hulls? (i.e. caves, overhangs, etc)
    * possibly use 3d textures with extents instead of absolutes?

# Algorithm Functional Blocks:
------------------------------
* PGM
  * Requires: (view-dependent)
    * reference sphere(s) : primary and secondary reference spheres
    * sampling camera
    * deferred texturing
  * Produces:
    * sphere normals 
    * positions
  * Create gridpoints which span the viewport
  * Computes ray-sphere intersection point & sphere intersection normal
    * gamma1 = asin((d/r) sin w) - w : first intersection angle from nadir
    * gamma2 = -asin((d/r) sin w)- w + pi : second intersection angle from nadir
  * pertinent area
    * occluded area (beyond horizon and determined by maximum height)
    * non-sampled intersecting area (below the frustum but should displace into the frustum)
  * Reference Spheres
  	* Primary and secondary (used for occluded area) only differ by radius
  	* Calculation of radii occurs on GPU before PGM
  	* Primary Reference Sphere:
  	  * radius calculated as minimum visible radius from previous frame?
  	    * using a mimimum mipmap operation on the gpu
  	    * final minimum read back to CPU and saved for next frame
  	* Secondary Reference Sphere:
  	  * tangent ray angle to primary sphere : tau = asin(r/d)
  	  * extent of occluded area is defined by maximum possible normal angle on maximum sphere
  	  	* gamma0 = -asin((d/rmax) sin tau) - tau + pi
                 = -asin((d/rmax) sin (asin(r/d))) - asin(r/d) + pi
  	  * secondary sphere radius : rs = d * cos(gamma0)
  	  * can replace the planet with an impostor when returned radius for secondary sphere is negative
  * Sampling Camera:
    * eye space : relative to camera; view matrix transforms from world-space to eye-space
    * projection matrix transforms points from eye-space into canonical view volume:
      * frustum (field of view, aspect ratio, near plane, far plane) becomes cube
      * all visible objects exist in the cube defined by (-1,-1,-1) and (1,1,1)
    * sampling camera has same position as view camera, but different orientation and frustum
    * sampling frustum contains parts of sphere that should be sampled
    * need to find 3 basis vectors which define sampling camera orientation & matrix
    * need to find frustum attributes to get projection matrix 
      * inverse of projection matrix is used by PGM to convert grid point to eye ray
    * calculate the corner rays of the view frustum and the visible sphere frustum
      * view frustum : easy, just use near, left, right, top, bottom
      * visible sphere frustum : 
        * calculate orientation difference from view frustum
          * backward vector : b = -n (nadir)
          * right vector : r = ||n x f|| (f = camera forward vector)
          * up vector : u = ||r x n||
        * central axis of frustum is nadir
        * intersection of each frustum plane and sphere gives tangent vector; tangent vector intersection with near plane gives half-side-length of near plane
          * h = near * atan(tau), tau = tangent vector angle
        * then the four corner rays are:
          * (-h,-h,n)
          * (h,-h,n)
          * (h,h,n)
          * (-h,h,n)
      * intersection between view frustum and visible sphere frustum:
        * transform corner rays for both frusta into same coordinate system
        * use rotated eye-space : rotate viewing camera so that yz-plane includes the center of the planet
          * ensures top edge of sampling camera frustum is parallel to horizon
        * calculate normals of frustum planes in frusta A and B
          * corner vector cross products: e.g. n2 = ||n3 x n2||, n1 = ||n2 x n1||
        * intersect the planes of A with the planes of B
          * cross product between plane normals
          * rays are then checked against the opposite planes, 2 from A and 2 from B
        * find the rays of A that exist in B
          * test against planes of B & add to intersection list
        * find the rays of B that exist in A
          * test against planes of A & add to intersection list
      * Sampling camera matrix : S = O X R X V, V = view matrix, R = rotated eye matrix, O = rotated eye-space to sampling camera space
        * O is generated from the set of intersection rays; forward vector orients near plane
          * forward vector is calculated as the average of the intersection rays
          * sampling camera must be oriented with horizon : yz-plane must include center of planet
            * => y,z components of forward vector are averaged from y,z of intersection rays; x is set to 0
          * basis vectors for sampling cameera orientation:
            * b = -f
            * r = ||b X n||
            * u = ||b X r||
          * ray intersection with near plane : t = n/ray_z; Point = t*ray
  * PGM on GPU:
    * conceptually:
      * for every grid point, eye ray is generated
      * calculate local coord system to generate sphere normals from normal angle
      * angle of eye to nadir is calculated
      * angle is used to calculate the normal angle at the ray-circle intersection point
        * need 3 basis vectors which describe points and vectors relative to the plane of the cross section
      * calculate the sphere normal in eye space using eye-space basis vectors
      * normal angle is used to calculate the position of the ray-circle intersection
    * actually:
      * bind two target buffers : sphere normals and sphere positions in sampling camera space
        * 32-bit FP RGB textures
      * sampling camera matrix (SCM) and its inverse (SCMI) are uploaded
      * In the fragment shader:
        * find eye ray associated with current fragment : fragment coord is transformed to clip space coord ([-1 to 1] on all axes; represents everything in frustum) using linear transformations
        * SCMI transforms clip space to sampling camera coord
        * perspective divide
        * position of grid point in sampling coords is the vector from eye to grid point (so it is then normalized)
        * calc right vector from cross product of eye vector and nadir, transformed using SCM
        * need 3 normalized basis vectors : i,j,k
        * i is nadir
        * j is cross product of i and the right vector
        * k is the cross product of j and i
        * calculate the ray angle (w.r.t. n) w = asin(dot(r,k))
        * precompute c = dist/radius, let u = dot(r,k)
        * gamma1 = asin(c*u) - asin(u)
        * user defined constant for % of range of positive ray values for interp : zeta = [0,1]
        * calc blending factor to mix two reference spheres' intersection results : 
          * f = (w-tau0)/(b-tau0), b = zeta*tau0
          * use blend factor to calc ray to intersect secondary reference sphere; it mixes between the beginning ray angle and the angle of the ray tangent to the secondary reference sphere
          * ray vector created is used in dot product with the up vector in local coord sys
          * calc normal angle with new ray and secondary reference sphere
          * use blend factor again to mix between normal angle from primary and normal angle from secondary
        * sphere normal s = a * cos(gamma) + k * sin(gamma), a = antinadir, k = up vector, gamma = blended normal angle; s will be vector in eye space
* Height Composition
  * produces accumulated heights based on all height datasets for each projected grid point
  * uses sphere normals to sample datasets
* Rasterization
  * uses accumulated heights to displace mesh and render
* Color Composition
  * accumulates colors and surface normals
* Atmosphere, lighting, etc.

# Questions:
------------
* How well does this work at long range?
* Can it automatically scale the number of grid points based on distance?

# Unresolved Issues:
--------------------