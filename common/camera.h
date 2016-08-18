#ifndef CAMERA_H
#define CAMERA_H

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\transform.hpp>
#include <glm4\bivector.hpp>
#include <glm4\spinor.hpp>

class Camera
{
public:
	/**
	* 3D Camera constructor.
	* \param position 3D vector.
	* \param fov Field of view in radians.
	* \param aspect Width to Height ratio.
	* \param zNear
	* \param zFar
	* \param forward Camera direction (default: [0,0,-1]).
	*/
	Camera(const glm::vec3& position, float fov, float aspect, float zNear, float zFar, const glm::vec3& forward = glm::vec3(0.0f, 0.0f, -1.0f));

	/**
	 * Rotate among UP^RIGHT plane.
	 * \param angle in radians
	 */
	void roll(const float angle);

	/**
	* Rotate among FORWARD^UP plane.
	* \param angle in radians
	*/
	void pitch(const float angle);

	/**
	* Rotate among FORWARD^RIGHT plane.
	* \param angle in radians
	*/
	void yaw(const float angle);

	/**
	 * Rotate among "right" vector.
	 * \param angle 
	 */
	//void rotateVertical(const float angle);
	
	inline glm::mat4 getProjection() const
	{
		return _projection;
	}

	inline glm::mat4 getViewProjection() const
	{
		//glm::vec3 pos = _spinor.rotate(_position);
		glm::vec3 forw = _spinor.rotate(_forward);
		glm::vec3 up = _spinor.rotate(_up);
		glm::mat4 lookat = glm::lookAt(_position, _position + forw, up);
		return _projection * lookat;
	}

	inline glm::mat4 getView() const
	{
		//glm::vec3 pos = _spinor.rotate(_position);
		glm::vec3 forw = _spinor.rotate(_forward);
		glm::vec3 up = _spinor.rotate(_up);
		glm::mat4 lookat = glm::lookAt(_position, _position + forw, up);
		return lookat;
	}

	/**
	*	Wygenerowanie relatywnej pozycji wzglêdem kamery, traktuj¹ce pozycjê jako pocz¹tek uk³adu 
	* wspó³rzêdnych, obróconego w ten sposób, ¿e kierunek patrzenia pokrywa siê z osi¹ Z (w stronê 
	* ujemnych wartoœci, UP z osi¹ Y, a prawa strona z osi¹ X.
	* @param t Relative translation.
	 */
	glm::vec3 getRelativePosition(const glm::vec3 t) const;

	inline glm::vec3 getPosition() const
	{
		return _position;
	}

	inline glm::vec3 getForward() const
	{
		return _forward;
	}

	void moveForward(float m);
	inline void moveUp(float s) { _position += s * _spinor.rotate(_up); }
	inline void moveRight(float s) { _position += s * glm::cross(_spinor.rotate(_forward), _spinor.rotate(_up)); }

private:
	//Camera(const Camera&) = delete;
	Camera & operator=(const Camera&) = delete;

	glm::mat4 _projection;

	glm::vec3 _position;

	glm::vec3 _forward;
	glm::vec3 _up;

	glm4::Spinor3D _spinor;
};

#endif
