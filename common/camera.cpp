#include "camera.h"

Camera::Camera(
	const glm::vec3& position, 
	float fov, float aspect, 
	float zNear, float zFar, 
	const glm::vec3& forward)
{
	_position = position;
	_forward = forward;
	_up = glm::vec3(0.0f, 1.0f, 0.0f);

	float w = 2.0f;
	float h = w / aspect;// .0f;
	float near = 0.0f;
	float far = 2.0f;
	//_projection = glm::ortho(0.0f, w, 0.0f, h, near, far);
	//_projection = glm::ortho(-w / 2.0f, w / 2.0f, -h / 2.0f, h / 2.0f, zNear, zFar);
	//glm::vec4 test0 = _projection * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	//glm::vec4 test1 = _projection * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//glm::vec4 test2 = _projection * glm::vec4(1.0f, 1.0f, 2.0f, 1.0f);
	_projection = glm::perspective(fov, aspect, zNear, zFar);
	//_projection = glm::perspective(fov, aspect, zNear, zFar);
}

void Camera::roll(const float angle)
{
	glm::vec3 fr = _spinor.rotate(_forward);
	glm::vec3 ur = _spinor.rotate(_up);
	glm::vec3 right = glm::cross(fr, ur);
	glm4::Spinor3D rot(angle, glm4::bivec3(ur, right));
	_spinor *= rot;
}

void Camera::pitch(const float angle)
{
	glm::vec3 fr = _spinor.rotate(_forward);
	glm::vec3 ur = _spinor.rotate(_up);
	glm4::Spinor3D rot(angle, glm4::bivec3(fr, ur));
	_spinor *= rot;
}

void Camera::yaw(const float angle)
{
	glm::vec3 fr = _spinor.rotate(_forward);
	glm::vec3 ur = _spinor.rotate(_up);
	glm::vec3 right = glm::cross(fr, ur);
	glm4::Spinor3D rot(angle, glm4::bivec3(right, fr));
	_spinor *= rot;
}

//void Camera::rotateHorizontal(const float angle)
//{
//	glm::quat q(cos(angle), 0.0f, sin(angle), 0.0f);
//	_position = glm::mat3_cast(q) * _position;
//	_forward = glm::mat3_cast(q) * _forward;
//	_up = glm::mat3_cast(q) * _up;
//}
//
//void Camera::rotateVertical(const float angle)
//{
//	glm::vec3 right = glm::cross(_forward, _up) * sin(angle);
//
//	glm::quat q(cos(angle), right.x, right.y, right.z);
//	_position = glm::mat3_cast(q) * _position;
//	_forward = glm::mat3_cast(q) * _forward;
//	_up = glm::mat3_cast(q) * _up;
//}

glm::vec3 Camera::getRelativePosition(const glm::vec3 t) const
{
	glm::vec3 right = glm::cross(_forward, _up);
	return _position + right * t.x + _up * t.y - _forward * t.z;
}

void Camera::moveForward(float m)
{
	_position += _spinor.rotate(_forward) * m;
}

//void Camera::moveForward(float m)
//{
//	_position += _spinor.rotate(_forward) * m;
//}