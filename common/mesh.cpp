#include "mesh.h"

unsigned int LineMesh::_counter = 0;

LineMesh::LineMesh(const LineIndexedModel& indexedModel)
{
	initLineMesh(indexedModel);
	_numIndices = indexedModel.indices.size();
	std::cout << "LineMesh created. Existing LineMeshes: " << ++_counter << std::endl;
}

void LineMesh::initLineMesh(const LineIndexedModel& model)
{
	_numIndices = model.indices.size();

	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, _vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, _vertexArrayBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertexArrayBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void LineMesh::drawLines() const
{
	glBindVertexArray(_vertexArrayObject);
	glDrawElements(GL_LINES, _numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

LineMesh::~LineMesh()
{
	glDeleteBuffers(NUM_BUFFERS, _vertexArrayBuffers);
	glDeleteVertexArrays(1, &_vertexArrayObject);
	std::cout << "LineMesh deleted. Existing LineMeshes: " << --_counter << std::endl;
}
