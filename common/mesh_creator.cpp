#include "mesh_creator.h"

LineIndexedModel MeshCreator::sqare_grid(unsigned int x_ticks, unsigned int y_ticks,
	float x_scale, float y_scale)
{
	LineIndexedModel model;

	// Vertical lines.
	for (unsigned i = 0; i <= x_ticks; i++)
		model.positions.push_back(glm::vec2((i * 2.0f / (float) x_ticks - 1.0f) * x_scale, y_scale));

	for (unsigned i = 0; i <= x_ticks; i++)
		model.positions.push_back(glm::vec2((i* 2.0f / (float) x_ticks - 1.0f) * x_scale, -y_scale));

	for (unsigned i = 0; i <= x_ticks; i++)
	{
		model.indices.push_back(i);
		model.indices.push_back(i + x_ticks + 1);
	}
	
	// Horizontal lines.
	for (unsigned i = 1; i < y_ticks; i++)
		model.positions.push_back(glm::vec2(-x_scale, (i * 2.0f / (float)y_ticks - 1.0f) * y_scale));

	for (unsigned i = 1; i < y_ticks; i++)
		model.positions.push_back(glm::vec2(x_scale, (i * 2.0f / (float)y_ticks - 1.0f) * y_scale));

	for (unsigned i = 1; i < y_ticks; i++)
	{
		model.indices.push_back(i + 2 * x_ticks + 1);
		model.indices.push_back(i + 2 * x_ticks + y_ticks);
	}

	model.indices.push_back(0);
	model.indices.push_back(x_ticks);
	model.indices.push_back(x_ticks + 1);
	model.indices.push_back(2 * x_ticks + 1);

	//for (unsigned int i = 0; i < model.positions.size(); i++)
	for (unsigned int i = 0; i < model.indices.size(); i+=2)
	{
		//std::cout << model.positions[i].x << " " << model.positions[i].y << std::endl;
		std::cout << model.indices[i] << " " << model.indices[i + 1] << std::endl;
	}

	return model;
}