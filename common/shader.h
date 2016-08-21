#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL\glew.h>
#include <glm\glm.hpp>

class Camera2D;
struct Transform2D;

const std::string VERTEX_SHADER_EXTENSION = ".vsh";
const std::string GEOMETRY_SHADER_EXTENSION = ".gsh";
const std::string FRAGMENT_SHADER_EXTENSION = ".fsh";

class Shader
{
public:
	void bind();
	virtual ~Shader();

	template<typename T>
	void update_uniform(T value, const std::string& uniform_name);

protected:
	Shader();
	GLuint _program;

	virtual void init(const std::string& fileName) = 0;
	GLuint createShader(const std::string& text, unsigned int type);
	std::string loadShader(const std::string& fileName);
	void checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

private:
	void operator=(const Shader& shader) = delete;
	Shader(const Shader& shader) = delete;
};

class BasicShader : public Shader
{
public:
	BasicShader(const std::string& fileName);
	virtual ~BasicShader();

	void update(const Transform2D& transform, const Camera2D& camera, const glm::vec3& color);
protected:
	virtual void init(const std::string& fileName);
private:
	static const unsigned int _NUM_SHADERS = 2;
	static const unsigned int _NUM_UNIFORMS = 3;
	GLuint _shaders[_NUM_SHADERS];
	GLuint _uniforms[_NUM_UNIFORMS];
};

class DeformationShader : public Shader
{
public:
	DeformationShader(const std::string& fileName);
	virtual ~DeformationShader();

	void update(const Transform2D& transform, const Camera2D& camera, const glm::vec3& color, 
		const float& deformation_coeficient);
protected:
	virtual void init(const std::string& fileName);
private:
	static const unsigned int _NUM_SHADERS = 2;
	static const unsigned int _NUM_UNIFORMS = 4;
	GLuint _shaders[_NUM_SHADERS];
	GLuint _uniforms[_NUM_UNIFORMS];
};


class ParticleShader : public Shader
{
public:
	ParticleShader(const std::string& fileName);
	virtual ~ParticleShader();

	void update(const Transform2D& transform, const Camera2D& camera, const glm::vec3& color,
		const float& deformation_coeficient);
protected:
	virtual void init(const std::string& fileName);
private:
	static const unsigned int _NUM_SHADERS = 2;
	static const unsigned int _NUM_UNIFORMS = 4;
	GLuint _shaders[_NUM_SHADERS];
	GLuint _uniforms[_NUM_UNIFORMS];
};

#endif