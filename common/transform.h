#ifndef TRANSFORM_INCLUDED_H
#define TRANSFORM_INCLUDED_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "camera.h"

struct Transform
{
public:
	/**
	Transform struct constructor.	
	\param pos translation vector.
	\param rot RPY vector (roll, pitch, yaw).
	\param scale scale among each axis, default (1,1,1).
	\note Forward vector is (0,0,1), up vector is (0,1,0).
	*/
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));

	/**
	Creates Trasform struct only with rotation.
	\param rotation RPY vector (roll, pitch, yaw).
	*/
	static Transform rotation(glm::vec3& rotation);
	/**
	Creates Trasform struct only with scale.
	*/
	static Transform scale(glm::vec3& scale);
	/**
	Creates Trasform struct only with translation.
	*/
	static Transform translation(glm::vec3& translation);

	/**
	Transforms given point.
	*/
	glm::vec3 operator () (const glm::vec3& point) const;

	/**
	Rotate given point.
	*/
	glm::vec3 rotatePoint(const glm::vec3& point) const;

	/**
	Transform point
	*/
	glm::vec3 transformPoint(const glm::vec3& point) const;


	/**
	Generates Model transformation matrix. I.e. translation + scaling + rotation.
	*/
	glm::mat4 getModel() const;

	///**
	//Generates Rotation matrix.
	//*/
	//glm::mat4 getRotationMat4() const;

	//glm::mat3 getRotationMat3() const;

	/**
	Generates Model-View-Projection matrix.
	*/
	glm::mat4 getMVP(const Camera& camera) const;

	/**
	Generates Model-View matrix.
	*/
	glm::mat4 getMV(const Camera& camera) const;

	glm::vec3* getPos() { return &pos_; }
	glm::vec3* getScale() { return &scale_; }

	void setPos(glm::vec3& pos) { this->pos_ = pos; }
	
	/*
	\param ax axis of rotation
	\param angleInRadians
	*/
	void rotate(const glm::vec3& ax, const float angleInRadians);

	/*
	Add rotation using spinor.
	\param spinor
	*/
	void rotate(const glm4::Spinor3D& spinor);

	/**
	Change scale values among each of axis.
	*/
	void setScale(glm::vec3& scale);

protected:
private:
	glm::vec3 pos_;
	glm::quat rot_;
	glm4::Spinor3D spinor_;
	//glm::vec3 rot_;
	glm::vec3 scale_;
};

#endif