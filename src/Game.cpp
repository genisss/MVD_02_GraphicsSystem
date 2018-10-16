#include "Game.h"
#include "extern.h"
#include "Shader.h"

void Game::init() {

	//******** OLD CODE **********//
    	
	//init graphics system
	graphics_system_.init();

	//load a shader
	graphics_system_.loadShader("phong", "data/shaders/phong.vert", "data/shaders/phong.frag");
	graphics_system_.temp_texture = graphics_system_.loadTexture("data/assets/test.tga");

	int ent_plane = ECS.createEntity("plane");
	Mesh& pmc = ECS.createComponentForEntity<Mesh>(ent_plane);
	graphics_system_.createPlaneGeometry(pmc.vao, pmc.num_tris);	

	//******** NEW CODE **********//

	//TODO:
	// - store plane geometry in graphics system
	// - create material and store it in graphics system
	//    - texture id
	//    - shader id
	//    - all other properties
	// - change mesh component to store ids of geometry and material (in graphics system storage)
	// - change graphics system component render to use geometry and material of component
	// - add a function to GraphicsSystem to parse an OBJ file and store it's geometry

}

//Entry point for game update code
void Game::update(float dt) {

	graphics_system_.update(dt);

}

