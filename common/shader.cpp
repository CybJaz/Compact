#include "shader.h"

#include <fstream>
#include <iostream>

#include "transform_2d.h"

#define SHADER_DIR "../common/shaders/"

Shader::Shader()
{
	_program = glCreateProgram();
}

Shader::~Shader()
{
	glDeleteProgram(_program);
}
void Shader::bind()
{
	glUseProgram(_program);
}

std::string Shader::loadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

GLuint Shader::createShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type);

	if (shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

	const GLchar* p[1];
	p[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, p, lengths);
	glCompileShader(shader);

	checkShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

	return shader;
}

BasicShader::BasicShader(const std::string& fileName)
{
	init(fileName);
}

BasicShader::~BasicShader()
{
	for (unsigned int i = 0; i < _NUM_SHADERS; i++)
	{
		glDetachShader(_program, _shaders[i]);
		glDeleteShader(_shaders[i]);
	}
}

void BasicShader::update(const Transform2D& transform, const Camera2D& camera, const glm::vec3& color)
{
	glm::mat4 Model = transform.getModel();
	glm::mat4 MVP = transform.getMVP(camera);
	//glm::mat4 MVP = transform.getMVP(camera);
	//glm::mat4 Normal = transform.getModel();

	glUniformMatrix4fv(_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(_uniforms[1], 1, GL_FALSE, &Model[0][0]);
	glUniform3f(_uniforms[2], color[0], color[1], color[2]);
}

void BasicShader::init(const std::string& fileName)
{
	_shaders[0] = createShader(loadShader(SHADER_DIR + fileName + VERTEX_SHADER_EXTENSION), GL_VERTEX_SHADER);
	_shaders[1] = createShader(loadShader(SHADER_DIR + fileName + FRAGMENT_SHADER_EXTENSION), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < _NUM_SHADERS; i++)
		glAttachShader(_program, _shaders[i]);

	glBindAttribLocation(_program, 0, "position");
	//glBindAttribLocation(_program, ATTRIB_VB, "color");

	glLinkProgram(_program);
	checkShaderError(_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(_program);
	checkShaderError(_program, GL_LINK_STATUS, true, "Invalid shader program");

	_uniforms[0] = glGetUniformLocation(_program, "MVP");
	_uniforms[1] = glGetUniformLocation(_program, "Model");
	_uniforms[2] = glGetUniformLocation(_program, "Color");
}


DeformationShader::DeformationShader(const std::string& fileName)
{
	init(fileName);
}

DeformationShader::~DeformationShader()
{
	for (unsigned int i = 0; i < _NUM_SHADERS; i++)
	{
		glDetachShader(_program, _shaders[i]);
		glDeleteShader(_shaders[i]);
	}
}

void DeformationShader::update(const Transform2D& transform, const Camera2D& camera,
	const glm::vec3& color, const float& deformation_coefficient)
{
	glm::mat3 ModelView = transform.get_model_view3(camera);
	glm::mat4 Projection = camera.get_projection();
	//glm::mat4 MVP = transform.getMVP(camera);
	//glm::mat4 Normal = transform.getModel();

	glUniformMatrix3fv(_uniforms[0], 1, GL_FALSE, &ModelView[0][0]);
	glUniformMatrix4fv(_uniforms[1], 1, GL_FALSE, &Projection[0][0]);
	glUniform3f(_uniforms[2], color[0], color[1], color[2]);
	glUniform1f(_uniforms[3], deformation_coefficient);
}

void DeformationShader::init(const std::string& fileName)
{
	_shaders[0] = createShader(loadShader(SHADER_DIR + fileName + VERTEX_SHADER_EXTENSION), GL_VERTEX_SHADER);
	_shaders[1] = createShader(loadShader(SHADER_DIR + fileName + FRAGMENT_SHADER_EXTENSION), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < _NUM_SHADERS; i++)
		glAttachShader(_program, _shaders[i]);

	glBindAttribLocation(_program, 0, "position");
	//glBindAttribLocation(_program, ATTRIB_VB, "color");

	glLinkProgram(_program);
	checkShaderError(_program, GL_LINK_STATUS, true, "Error linking shader program");

	glValidateProgram(_program);
	checkShaderError(_program, GL_LINK_STATUS, true, "Invalid shader program");

	_uniforms[0] = glGetUniformLocation(_program, "MV");
	_uniforms[1] = glGetUniformLocation(_program, "Projection");
	_uniforms[2] = glGetUniformLocation(_program, "Color");
	_uniforms[3] = glGetUniformLocation(_program, "Coefficient");
}
