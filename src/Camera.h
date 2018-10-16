//
//  Camera.h
//
//  Copyright © 2018 Alun Evans. All rights reserved.
//
#pragma once
#include "includes.h"

using namespace lm;

class Camera {
public:
	Camera();

	//updates view matrix from member variables
	void update();

	void lookAt(const vec3 eye, const vec3 target, const vec3 up);
	void setOrthographic(float left, float right, float bottom, float top, float near, float far);
	void setPerspective(float fov_rad, float aspect, float near, float far);

	//setters for new matrix variables
	//we expose these as we will be changeing the view matrix frequently
	void eye(const vec3 eye) { eye_ = eye; update(); }
	void target(const vec3 target) { target_ = target; update(); }
	void up(const vec3 up) { up_ = up; update(); }

	//getters for main camera variables
	const vec3 eye() { return eye_; }
	const vec3 target() { return target_; }

	//gets view projection matrix
	mat4 ViewProjection() { return projection_matrix_ * view_matrix_; }


private:
	lm::vec3 eye_;
	lm::vec3 target_;
	lm::vec3 up_;
	lm::mat4 view_matrix_;
	lm::mat4 projection_matrix_;
};
