This document covers the implementation details and challenges/questions for the integration of the GDAL library which allows for the processing, accessing, and archiving of NASA's planetary datasets.  

Goals:
------
* integrate GDAL into our simulator codebase
* automatically get relevant datasets (mars, moon, earth)
* handles multiple datasets simultaneously (high-res local, low-res global)
* handles multiple projections simultaneously (polar, equirectangular, etc.)
* able to load multiple types of data:
  * height maps
  * color maps
  * false-color (infrared etc.)
  * atmospheric data (at different levels)

Extensions:
-----------

Implementation:
---------------

Questions:
----------

Unresolved Issues:
------------------