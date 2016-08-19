#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <GL\glew.h>
#include <glm\glm.hpp>

class Camera;
//struct Transform;

const std::string VERTEX_SHADER_EXTENSION = ".vsh";
const std::string GEOMETRY_SHADER_EXTENSION = ".gsh";
const std::string FRAGMENT_SHADER_EXTENSION = ".fsh";

class Shader
{
public:
	void bind();
	virtual ~Shader();

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

	void update(const Transform& transform, const Camera& camera);
protected:
	virtual void init(const std::string& fileName);
private:
	static const unsigned int _NUM_SHADERS = 2;
	static const unsigned int _NUM_UNIFORMS = 3;
	GLuint _shaders[_NUM_SHADERS];
	GLuint _uniforms[_NUM_UNIFORMS];
};

#endif