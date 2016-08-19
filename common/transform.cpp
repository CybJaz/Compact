#include "transform.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale)
{
	this->pos_ = pos;
	
	glm4::Spinor3D roll(rot[0], glm4::bivec3(0.0f, 0.0f, 1.0f));
	glm4::Spinor3D pitch(rot[1], glm4::bivec3(1.0f, 0.0f, 0.0f));
	glm4::Spinor3D yaw(rot[2], glm4::bivec3(0.0f, 1.0f, 0.0f));
	
	spinor_ = yaw * pitch * roll;

	this->scale_ = scale;
}

Transform Transform::rotation(glm::vec3& rotation)
{
	return Transform(glm::vec3(), rotation);
}

Transform Transform::scale(glm::vec3& scale)
{
	return Transform(glm::vec3(), glm::vec3(), scale);
}

Transform Transform::translation(glm::vec3& translation)
{
	return Transform(translation);
}

glm::vec3 Transform::operator () (const glm::vec3& point) const
{
	return glm::vec3(getModel() * glm::vec4(point, 1.0f));
}

glm::vec3 Transform::rotatePoint(const glm::vec3& point) const
{
	return spinor_.toMat3() * point;
}

glm::vec3 Transform::transformPoint(const glm::vec3& point) const
{
	return glm::vec3(getModel() * glm::vec4(point, 0.0f));
}

glm::mat4 Transform::getModel() const
{
	glm::mat4 posMat = glm::translate(pos_);
	glm::mat4 scaleMat = glm::scale(scale_);

	return posMat * spinor_.toMat4() * scaleMat;
}

//glm::mat4 Transform::getRotationMat4() const
//{
//	return spinor_.toMat4();
//}
//
//glm::mat3 Transform::getRotationMat3() const
//{
//	return spinor_.toMat3();
//}

glm::mat4 Transform::getMVP(const Camera& camera) const
{
	glm::mat4 VP = camera.getViewProjection();
	glm::mat4 M = getModel();

	return VP * M;//camera.GetViewProjection() * GetModel();
}

glm::mat4 Transform::getMV(const Camera& camera) const
{
	glm::mat4 V = camera.getView();
	glm::mat4 M = getModel();

	return V * M;//camera.GetViewProjection() * GetModel();
}

void Transform::rotate(const glm::vec3& ax, const float ang)
{
	spinor_ *= glm4::Spinor3D(ang, glm4::bivec3(ax));
};

void Transform::rotate(const glm4::Spinor3D& spinor)
{
	spinor_ *= spinor;
};

void Transform::setScale(glm::vec3& scale) { this->scale_ = scale; }