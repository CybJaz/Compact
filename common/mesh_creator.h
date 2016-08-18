#ifndef MESH_CREATOR_H
#define MESH_CREATOR_H

#include "mesh.h"

class MeshCreator
{
public:
	static LineIndexedModel sqare_grid(unsigned int x_ticks, unsigned int y_ticks, 
		float x_scale = 1.0f, float y_scale = 1.0f);
private:
	void operator=(const MeshCreator& mesh) = delete;
	MeshCreator(const MeshCreator& mesh) = delete;

};

#endif
