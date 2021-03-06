#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <vector>

struct LineIndexedModel
{
	std::vector<glm::vec2> positions;
	std::vector<unsigned int> indices;
};

class LineMesh
{
	static unsigned int _counter;
public:
	LineMesh(const LineIndexedModel& indexedModel);
	void drawLines() const;
	void drawPoints() const;
	
	template <GLenum DRAWING_MODE>
	void draw() const;

	~LineMesh();

private:
	static const unsigned int NUM_BUFFERS = 2;
	void operator=(const LineMesh& mesh) = delete;
	LineMesh(const LineMesh& mesh) = delete;

	void initLineMesh(const LineIndexedModel& model);

	GLuint _vertexArrayObject;
	GLuint _vertexArrayBuffers[NUM_BUFFERS];
	unsigned int _numIndices;
};

template <GLenum DRAWING_MODE>
void LineMesh::draw() const
{
	glBindVertexArray(_vertexArrayObject);
	glDrawElements(DRAWING_MODE, _numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

#endif