#include "Camera.h"
#include <glm/ext.hpp>

#include <iostream>

const glm::vec3 Camera::center_position() const
{
	return  glm::vec3(position_.x + front_dir_.x,
		position_.y + front_dir_.y,
		position_.z + front_dir_.z);
}

void Camera::move_forward(float delta)
{
	position_ += delta * front_dir_;
}

void Camera::move_backward(float delta)
{
	move_forward(-delta);
}

void Camera::move_left(float delta)
{
	position_ -= delta * right_dir_;
}

void Camera::move_right(float delta)
{
	move_left(-delta);
}

void Camera::rotate_left(float delta)
{


	glm::mat4 yawMat(1);
	yawMat = glm::rotate(yawMat, delta, up_dir_);
	right_dir_ = glm::vec3(yawMat*glm::vec4(right_dir_, 1.0));
	right_dir_ = glm::normalize(right_dir_);
	right_dir_.y = 0;
	up_dir_ = glm::normalize(glm::cross(right_dir_, front_dir_));
	front_dir_ = glm::normalize(glm::cross(up_dir_, right_dir_));

}

void Camera::rotate_up(float delta)
{
	glm::mat4 pitchMat(1);
	pitchMat = glm::rotate(pitchMat, delta, right_dir_);
	up_dir_ = glm::vec3(pitchMat*glm::vec4(up_dir_, 1.0));
	// center_position_ = position_+look_at_direction();

	up_dir_ = glm::normalize(up_dir_);
	right_dir_ = glm::normalize(glm::cross(front_dir_, up_dir_));
	right_dir_.y = 0;
	front_dir_ = glm::normalize(glm::cross(up_dir_, right_dir_));
}

void Camera::rotate_right(float delta)
{
	rotate_left(-delta);
}