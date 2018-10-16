#version 330

layout(location = 0) in vec3 a_vertex;
layout(location = 1) in vec2 a_uv;
layout(location = 2) in vec3 a_normal;

uniform mat4 u_mvp;
uniform mat4 u_model;
uniform mat4 u_normal_matrix;
uniform vec3 u_light_pos; // in world space
uniform vec3 u_cam_pos; // in world space

out vec2 v_uv;
out vec3 v_normal;
out vec3 v_light_dir;
out vec3 v_cam_dir;

void main(){

	v_uv = a_uv;
	//rotate normal 
	v_normal = (u_normal_matrix * vec4(a_normal, 1.0)).xyz;

	//calculate world position of current vertex
	vec3 vertex_world_pos = (u_model * vec4(a_vertex, 1.0)).xyz;

	//calculate direction to light in world space
	v_light_dir = u_light_pos - vertex_world_pos;

	//calculate direction to camera in world space
	v_cam_dir = u_cam_pos - vertex_world_pos;

	gl_Position = u_mvp * vec4(a_vertex, 1.0);
}