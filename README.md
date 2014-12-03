simulation
==========

This project has the goal of developing a modular, fast, and realistic multi-domain system simulation and renderer.  It is the culmination of graphics, rendering, and physics research that I've done over the past 6-7 years as either hobby or class projects.

We are developing a GPU-accelerated muti-domain physics simulation and realistic rendering system.  It will support the simulation of multiple physical domains simultaneously, e.g. fluid dynamics, gravity, elastic and inelastic collision.  Additionally, it will support realistic rendering of the simulation as feedback and interaction with the user.  We are integrating support for procedural data generation as well as the data stored by NASA in its Planetary Data System.  By working with existing high-fidelity physics simulation engines and integrating the latest in GPU acceleration and rendering techniques, we will provide real-time simulation and feedback to the user.

Research:
---------
Much of the research for this engine has not been documented (sadly), as it has been ongoing since around 2008.  Some of that resarch culminated in a class project which was a networked first person shooter built on top of a custom software rendering engine (also on github).  However, Since this project has started, all resarch has been (as much as possible) catalogued by topic as [book references](BookReferences.md), [web references](WebReferences.md), or the relevant research papers have been committed into the repository under the Docs folder.  Additionally, any ongoing or past implementation notes are committed as their own files, listed under Relevant Documentation in this file.

Design:
-------

Implementation:
---------------

Relevant Documentation:
-----------------------
* [The current Design Document](Design.md)
* [A list of book chapters relevant to each subject](BookReferences.md)
* [A list of websites relevant to each subject](WebReferences.md)
* [A development document full of notes for implementing and extending PGM](PGMImplementation.md)
* [A development document with notes for the possible integration of GDAL](GDALIntegration.md)