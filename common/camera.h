#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\transform.hpp>

class Camera
{
public:
	/**
	* Camera constructor.
	* \param position 2D vector
	* \param aspect Width to Height ratio.
	*/
	Camera(const glm::vec3& position, float aspect, float height = 2.0f);

	inline glm::mat4 getProjection() const
	{
		return _projection;
	}

	//inline glm::mat4 getViewProjection() const
	//{
	//	//glm::vec3 pos = _spinor.rotate(_position);
	//	glm::vec3 forw = _spinor.rotate(_forward);
	//	glm::vec3 up = _spinor.rotate(_up);
	//	glm::mat4 lookat = glm::lookAt(_position, _position + forw, up);
	//	return _projection * lookat;
	//}

	//inline glm::mat4 getView() const
	//{
	//	//glm::vec3 pos = _spinor.rotate(_position);
	//	glm::vec3 forw = _spinor.rotate(_forward);
	//	glm::vec3 up = _spinor.rotate(_up);
	//	glm::mat4 lookat = glm::lookAt(_position, _position + forw, up);
	//	return lookat;
	//}


private:
	//Camera(const Camera&) = delete;
	Camera & operator=(const Camera&) = delete;

	glm::mat4 _projection;

	glm::vec2 _position;
};

#endif
