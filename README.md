### La Salle - Universitat Ramon Llull : Master in Advanced Videogame Development
## Engine Programming - 02-Graphics System

### TODO
#### in GraphicsSystem.h and GraphicsSystem.cpp
 - fill properties of Material and Geometry structs
 - create storage vectors for Materials and Geometries
 - create function to create/access/update materials and geometries
 - optional: modify createPlaneGeometry so it creates a geometry directly in storage vector
 - change render function so it uses geometry and material of component

#### in Component.h
 - modify Mesh Component to store
		- index of geometry (in graphics system storage array)
		- index of material (in graphics system storage array)

#### in Game.cpp
 - Create plane geometry and store in graphics system
 - create material and store it in graphics system
 - Create Entity for plane (as code above)
 - Create Mesh Component for plane (modified from code above) to set material and geometry correctly

#### Advanced
 - add a function to GraphicsSystem to parse an OBJ file and store it's geometry

