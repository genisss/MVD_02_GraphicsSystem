//
//  Camera.cpp
//
//  Copyright © 2018 Alun Evans. All rights reserved.
//
#include "Camera.h"

// default constructor which creates placeholder matrices
Camera::Camera() {
	eye_ = vec3(0.0f, 0.0f, 1.0f); target_ = vec3(0.0f, 0.0f, 0.0f); up_ = vec3(0.0f, 1.0f, 0.0f);
	view_matrix_.lookAt(eye_, target_, up_); 
	projection_matrix_.perspective(60.0f*DEG2RAD, 1, 0.01f, 100.0f); 
}

// updates view matrix according to current member variables
void Camera::update() {
	view_matrix_.lookAt(eye_, target_, up_);
}

// sets member variables and sets view matrix
void Camera::lookAt(const vec3 eye, const vec3 target, const vec3 up) {
	eye_ = eye; 
	target_ = target;
	up_ = up;
	view_matrix_.lookAt(eye_, target_, up_);
}

//set camera projection matrix using orthographic projection
void Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far) {
	projection_matrix_.orthographic(left, right, bottom, top, near, far);
}

//sets camera projection matrix using perspective projection
void Camera::setPerspective(float fov_rad, float aspect, float near, float far) {
	projection_matrix_.perspective(fov_rad, aspect, near, far);
}