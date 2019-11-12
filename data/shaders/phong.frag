#version 330

in vec2 v_uv;
in vec3 v_normal;
in vec3 v_light_dir;
in vec3 v_cam_dir;

out vec4 fragColor;

uniform vec3 u_diffuse;
uniform sampler2D u_texture_diffuse;
uniform float u_glossiness;

void main(){

	vec3 diffuse_texture_color = texture(u_texture_diffuse, v_uv).xyz;

	//ambient light
	vec3 final_color = vec3(0.1, 0.1, 0.1) * diffuse_texture_color;
	
	vec3 L = normalize(v_light_dir); //to light
	vec3 N = normalize(v_normal); //normal
	vec3 R = reflect(-L,N); //reflection vector
	vec3 V = normalize(v_cam_dir); //to camera

	//diffuse color
	float NdotL = max(0.0, dot(N, L));
	vec3 diffuse_color = NdotL * texture(u_texture_diffuse, v_uv).xyz * u_diffuse;

	//specular color
	float RdotV = max(0.0, dot(R, V)); //calculate dot product
	RdotV = pow(RdotV, u_glossiness); //raise to power for glossiness effect
	vec3 specular_color = RdotV * vec3(1.0, 1.0, 1.0); //white specular colour

	//final color
	final_color += diffuse_color + specular_color;

	fragColor = vec4(final_color, 1.0);
}