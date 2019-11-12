#include <iostream>
#include <sstream>
#include <fstream>
#include "GraphicsSystem.h"
#include "Game.h"
#include "extern.h"

//destructor
GraphicsSystem::~GraphicsSystem() {
	//delete shader pointers
	for (auto shader_pair : shaders_) {
		if (shader_pair.second)
			delete shader_pair.second;
	}
}


void GraphicsSystem::init() {
	//set 'background' colour of framebuffer
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

}

void GraphicsSystem::update(float dt) {

	//set initial OpenGL state
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//TODO: remove this line!
	current_program_ = shaders_["phong"]->program;

	//get reference to mesh components first
	auto& mesh_components = ECS.getAllComponents<Mesh>();

	//loop over mesh components by reference (&)
	for (auto& curr_comp : mesh_components) {
		//Get shader frot his material
		Material& comp_mat = getMaterial(curr_comp.material);
		current_program_ = comp_mat.shader_id;

		//activate shader
		glUseProgram(current_program_);

		//render component
		renderMeshComponent_(curr_comp);
	}
}

//renders a given mesh component
void GraphicsSystem::renderMeshComponent_(Mesh& comp) {

	//TODO:
	// - modify this function so it renders the geometry and the material associated with the component
	Geometry& comp_geom = getGeometry(comp.geometry);
	Material& comp_mat = getMaterial(comp.material);

	lm::vec3 cam_position(3.0f, -3.0f, 3.0f);
	lm::vec3 cam_target(0.0f, 0.0f, 0.0f);
	lm::vec3 cam_up(0.0f, 1.0f, 0.0f);
	lm::mat4 view_matrix, projection_matrix, view_projection;
	view_matrix.lookAt(cam_position, cam_target, cam_up);
	projection_matrix.perspective(60.0f * DEG2RAD, 1, 0.01f, 100.0f);
	view_projection = projection_matrix * view_matrix;


	//get transform of components entity
	Transform& transform = ECS.getComponentFromEntity<Transform>(comp.owner);

	//model matrix
	lm::mat4 model_matrix = transform.getGlobalMatrix(ECS.getAllComponents<Transform>());

	//normal matrix
	lm::mat4 normal_matrix = model_matrix;
	normal_matrix.inverse();
	normal_matrix.transpose();

	//Model view projection matrix
	lm::mat4 mvp_matrix = view_projection * model_matrix;

	//ask shader for a reference to the uniforms 
	GLint u_mvp = glGetUniformLocation(current_program_, "u_mvp");
	GLint u_model = glGetUniformLocation(current_program_, "u_model");
	GLint u_normal_matrix = glGetUniformLocation(current_program_, "u_normal_matrix");
	GLint u_light_pos = glGetUniformLocation(current_program_, "u_light_pos");
	GLint u_cam_pos = glGetUniformLocation(current_program_, "u_cam_pos");
	GLint u_texture_diffuse = glGetUniformLocation(current_program_, "u_texture_diffuse");
	GLint u_glossiness = glGetUniformLocation(current_program_, "u_glossiness");
	GLint u_diffuse = glGetUniformLocation(current_program_, "u_diffuse");

	//if the uniforms exist, send the data to the shader
	if (u_mvp != -1) glUniformMatrix4fv(u_mvp, 1, GL_FALSE, mvp_matrix.m);
	if (u_model != -1) glUniformMatrix4fv(u_model, 1, GL_FALSE, model_matrix.m);
	if (u_normal_matrix != -1) glUniformMatrix4fv(u_normal_matrix, 1, GL_FALSE, normal_matrix.m);
	if (u_light_pos != -1) glUniform3f(u_light_pos, 1000.0f, 0.0f, 1000.0f); //... 3f - is 3 floats
	if (u_cam_pos != -1) glUniform3fv(u_cam_pos, 1, cam_position.value_); // ...3fv - is array of 3 floats
	if (u_texture_diffuse != -1) glUniform1i(u_texture_diffuse, 0); // ...1i - is integer
	if (u_glossiness != -1) glUniform1f(u_glossiness, 80.0f); //...1f - for float
	if (u_diffuse != -1) glUniform3fv(u_diffuse, 1, comp_mat.diffuse_color.value_); // ...3fv - is array of 3 floats

															  //activate texture unit 0, and bind our texture there
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, comp_mat.diffuse_texture);

	//tell OpenGL we want to the the vao_ container with our buffers
	glBindVertexArray(comp_geom.vao);

	//draw our geometry
	glDrawElements(GL_TRIANGLES, comp_geom.num_tris * 3, GL_UNSIGNED_INT, 0);

	//tell OpenGL we don't want to use our container anymore
	glBindVertexArray(0);

	glUseProgram(0);


}

//********************************************
// OpenGL helper functions
//********************************************

//loads a shader and returns its program id
int GraphicsSystem::loadShader(std::string name, std::string vs_path, std::string fs_path) {
	shaders_[name] = new Shader(vs_path, fs_path);
	return shaders_[name]->program;
}

//returns a shader program id
int GraphicsSystem::getShaderProgram(std::string name) {
	return shaders_[name]->program;
}

//loads a texture and returns its id
GLuint GraphicsSystem::loadTexture(std::string path) {
	Texture t("data/assets/test.tga");
	return t.texture_id;
}

//creates a standard plane geometry
int GraphicsSystem::createPlaneGeometry() {

	//TODO: 
	// - rewrite this function so that it adds a geometry to teh std::vector
	Geometry new_geom;


	//four vertices in a square
	const GLfloat position_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	//texture coordinates (uvs)
	const GLfloat texture_buffer_data[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};
	//normals point 'out of screen'
	const GLfloat normal_buffer_data[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};
	//index buffer
	const GLuint index_buffer_data[] = { 0, 1, 2, 0, 2, 3 };

	//set number of triangles (of passed variable)
	new_geom.num_tris = 2;

	//create Vertex Array Object
	glGenVertexArrays(1, &new_geom.vao);
	glBindVertexArray(new_geom.vao);

	//Now create Vertex Buffer Objects for each buffer: positions, uvs, normals, and indices
	GLuint vbo;
	//positions
	glGenBuffers(1, &vbo); //create buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //set OpenGL state to say we want to work with this buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(position_buffer_data), position_buffer_data, GL_STATIC_DRAW); //copy data
	glEnableVertexAttribArray(0); //enable attribute labelled as '0' in vertex shader: "layout(location = 0) in vec3 a_vertex;"
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //each attribute '0' is composed of 3 floats (in this case, xyz)

	//texture coords
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_buffer_data), texture_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1); // (1 = a_uv)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); //only two coordinates for textures: uv
	//normals								   
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normal_buffer_data), normal_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(2); // (2 = a_normal)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//indices
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_buffer_data), index_buffer_data, GL_STATIC_DRAW);
	//unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	geometries_.push_back(new_geom);

	//we'll return the index of the new geometry in the geometries aarray
	return (int)geometries_.size() - 1;
}

int GraphicsSystem::createBaseMaterial() {
	Material new_mat;
	materials_.push_back(new_mat);
	return (int)materials_.size() - 1;
}

int GraphicsSystem::createGeometryFromOBJ() {


	const string path = "data/assets/cube.obj";

	string line;
	ifstream myfile(path);
	if (myfile.is_open())
	{
		string vertex = "v";
		string texture = "vt";
		string normal = "vn";
		string faces = "f";
		std::vector <GLfloat> position_buffer_data_vector;
		std::vector <GLfloat> texture_buffer_data_vector;
		std::vector <GLfloat> normal_buffer_data_vector;
		std::vector <GLfloat> texture_buffer_data_provisional;
		std::vector <GLfloat> normal_buffer_data_provisional;
		std::vector <GLuint> index_buffer_data_vector;
		std::map <string, int> index_buffer_data_vector;
		size_t num_tris = 0;

		size_t num_vertex = 0;
		while (getline(myfile, line))
		{

			if (texture.length() <= line.length() && equal(texture.begin(), texture.end(), line.begin())) {
				std::istringstream iss(line);
				for (std::string line; iss >> line; ) {
					if (!line._Equal(texture)) {
						texture_buffer_data_provisional.push_back(stof(line));
					}
				}
			}
			else if (normal.length() <= line.length() && equal(normal.begin(), normal.end(), line.begin())) {
				std::istringstream iss(line);
				for (std::string line; iss >> line; ) {
					if (!line._Equal(normal)) {
						normal_buffer_data_provisional.push_back(stof(line));
					}
				}
			}
			//if line sarts with 'v' add toto std::vector of lm::vec3
			else if (vertex.length() <= line.length() && equal(vertex.begin(), vertex.end(), line.begin())) {
				std::istringstream iss(line);
				for (std::string line; iss >> line; ) {
					if (!line._Equal(vertex)) {
						position_buffer_data_vector.push_back(stof(line));
					}
				}
				num_vertex++;

			}
			else if (faces.length() <= line.length() && equal(faces.begin(), faces.end(), line.begin())) {
				num_tris++;
				std::istringstream iss(line);
				if (size(texture_buffer_data_vector) == 0) {
					texture_buffer_data_vector.resize(num_vertex * 2);
					normal_buffer_data_vector.resize(num_vertex * 3);
				}
				for (std::string line; iss >> line; ) {
					//Crear un nou stdMap<string,int>
					if (!line._Equal(faces)) {
						int idx_vert = (atoi(&line[0]) - 1);
						int idx_tex = (atoi(&line[2]) - 1) * 2;
						int idx_norm = (atoi(&line[4]) - 1) * 3;
						index_buffer_data_vector.push_back(idx_vert+1);
						texture_buffer_data_vector.at(idx_vert * 2) = texture_buffer_data_provisional.at(idx_tex);
						texture_buffer_data_vector[(idx_vert * 2) + 1] = texture_buffer_data_provisional[idx_tex + 1];
						normal_buffer_data_vector[idx_vert * 3] = normal_buffer_data_provisional[idx_norm];
						normal_buffer_data_vector[(idx_vert * 3) + 1] = normal_buffer_data_provisional[idx_norm + 1];
						normal_buffer_data_vector[(idx_vert * 3) + 2] = normal_buffer_data_provisional[idx_norm + 2];
					}
				}
			}

		}

		myfile.close();

		Geometry new_geom;

		//set number of triangles (of passed variable)
		new_geom.num_tris = num_tris;

		//create Vertex Array Object
		glGenVertexArrays(1, &new_geom.vao);
		glBindVertexArray(new_geom.vao);

		//Now create Vertex Buffer Objects for each buffer: positions, uvs, normals, and indices
		GLuint vbo;
		//positions
		glGenBuffers(1, &vbo); //create buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbo); //set OpenGL state to say we want to work with this buffer
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * position_buffer_data_vector.size(), &position_buffer_data_vector[0], GL_STATIC_DRAW); //copy data
		glEnableVertexAttribArray(0); //enable attribute labelled as '0' in vertex shader: "layout(location = 0) in vec3 a_vertex;"
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0); //each attribute '0' is composed of 3 floats (in this case, xyz)

		//texture coords
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * texture_buffer_data_vector.size(), &texture_buffer_data_vector[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1); // (1 = a_uv)
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0); //only two coordinates for textures: uv
		//normals								   
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * normal_buffer_data_vector.size(), &normal_buffer_data_vector[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2); // (2 = a_normal)
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		//indices
		GLuint ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * index_buffer_data_vector.size(), &index_buffer_data_vector[0], GL_STATIC_DRAW);
		//unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		geometries_.push_back(new_geom);

		//we'll return the index of the new geometry in the geometries aarray
		return (int)geometries_.size() - 1;



	}

	else cout << "Unable to open file";

	return 0;
}

