#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm\glm.hpp>

struct Particle
{
	glm::vec2 position;

	inline void move(glm::vec2 vec) { position += vec; }
}

#endif