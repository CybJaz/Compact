#include "mesh_creator.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

LineIndexedModel MeshCreator::sqare_grid(unsigned int x_ticks, unsigned int y_ticks,
	float x_scale, float y_scale)
{
	LineIndexedModel model;

	if (x_ticks < 2)
		x_ticks = 2;

	if (y_ticks < 2)
		y_ticks = 2;

	// First horizontal line.
	model.positions.push_back(glm::vec2(-x_scale, y_scale));
	for (unsigned int j = 1; j < x_ticks; j++)
	{
		glm::vec2 point = glm::vec2((j * 2.0f / (float) (x_ticks - 1) - 1.0f) * x_scale, y_scale);
		model.positions.push_back(point);
		//std::cout << point.x << " " << point.y << std::endl;
		model.indices.push_back(j - 1);
		model.indices.push_back(j);
	};

	for (unsigned int i = 1; i < y_ticks; i++)
	{
		model.positions.push_back(
			glm::vec2(-x_scale, (i * 2.0f / (float)(y_ticks - 1) - 1.0f) * -y_scale));
		//std::cout << -x_scale << " " << (i * 2.0f / (float)(y_ticks - 1) - 1.0f) * -y_scale << std::endl;
		model.indices.push_back((i - 1) * x_ticks);
		model.indices.push_back(i * x_ticks);

		for (unsigned int j = 1; j < x_ticks; j++)
		{
			glm::vec2 point((j * 2.0f / (float)(x_ticks -1) - 1.0f) * x_scale, 
							(i * 2.0f / (float)(y_ticks - 1) - 1.0f) * -y_scale);

			model.positions.push_back(point);
			//std::cout << point.x << " " << point.y << std::endl;

			// vertical line
			model.indices.push_back((i-1) * x_ticks + j);
			model.indices.push_back(i * x_ticks + j);

			// horizontal line
			model.indices.push_back(i * x_ticks + j - 1);
			model.indices.push_back(i * x_ticks + j);
		}
	}

	//for (int i = 0; i < model.indices.size(); i+=2)
	//{
	//	std::cout << model.indices[i] << " " << model.indices[i + 1] << std::endl;
	//}

	return model;
}

LineIndexedModel MeshCreator::circle_full_triangles(unsigned int sides, float scale)
{
	LineIndexedModel model;

	model.positions.push_back(glm::vec2(0.0f,0.0f));
	model.positions.push_back(glm::vec2(1.0f, 0.0f) * scale);

	unsigned int triangle = 0;
	float angle = glm::two_pi<float>() / (float) sides;

	unsigned int i = 1;
	for (; i < sides; i++)
	{
		model.positions.push_back(glm::vec2(glm::cos((float)i * angle), 
								glm::sin((float)i * angle)) * scale);
		model.indices.push_back(0);
		model.indices.push_back(i);
		model.indices.push_back(i+1);
	}

	model.indices.push_back(0);
	model.indices.push_back(i);
	model.indices.push_back(1);

	return model;
}