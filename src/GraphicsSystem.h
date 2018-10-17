#pragma once
#include "includes.h"
#include "Shader.h"
#include "Components.h"
#include "Texture.h"
#include <vector>
#include <unordered_map>

struct Geometry {
	//TODO:
	// - add properties
	// - add constructor with default properties
};

struct Material {
	//TODO:
	// - add properties here
	// - add constructor with default properties

	// hint: ONLY store simple data (floats, ints, vec3s etc)
	// do NOT store pointers to data
};


class GraphicsSystem {
public:
	//destructor frees Shader objects
	~GraphicsSystem();

	//init and update are mandatory for all systems
	void init();
	void update(float dt);

	//load and get shader into/from private unordered_map
	int loadShader(std::string name, std::string vs_path, std::string fs_path);
	int getShaderProgram(std::string name);

	//loads a texture and returns the OpenGL texture id
	GLuint loadTexture(std::string path);

	//TODO - change this (see description in cpp file)
	void createPlaneGeometry(GLuint& vao, GLuint& num_tris);

	//TODO: - implement this
	int createMaterial();

	//TODO 
	// - delete this!
	GLuint temp_texture;

private:
	//dictionary of shaders
	std::unordered_map<std::string, Shader*> shaders_;

	//TODO:
	// - create *private* storage vectors for Geometries and Materials
	// - and *public* methods for adding removing data to them
	


	//rendering
	GLuint current_program_;
	void renderMeshComponent_(Mesh& comp);
};
