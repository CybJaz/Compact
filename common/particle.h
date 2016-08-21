#ifndef PARTICLE_H
#define PARTICLE_H

#include <cstdlib>

#include <glm\glm.hpp>

#include "mesh.h"

struct Particle
{
	glm::vec2 position, direction;
	float speed, size;
	float life;

	inline void move(glm::vec2 vec) { position += vec; }
};

class ParticleSystem
{
public:
	ParticleSystem(const LineIndexedModel & particle_model);

	void update_buffer();
	void update_particles(float time_delta, float distortion_coefficient, const Camera2D& camera);
	void draw();

	void horizontal_set();
	void random_set();

private:
	//void initialize();

	unsigned int _particles_count;

	static const unsigned int MAX_PARTICLES = 200;
	Particle _particles[MAX_PARTICLES];

	// Particle mesh
	const unsigned int PARTICLE_MESH_BUFFERS = 2;
	GLuint _vertex_array_object; 
	GLuint _vertex_array_buffers[2];
	unsigned int _num_of_mesh_indices;

	GLuint _particles_position_buffer;

	GLfloat g_particule_position_size_data[MAX_PARTICLES * 3];
};

ParticleSystem::ParticleSystem(const LineIndexedModel & particle_model)
{
	random_set();

	_num_of_mesh_indices = particle_model.indices.size();

	glGenVertexArrays(1, &_vertex_array_object);
	glBindVertexArray(_vertex_array_object);

	glGenBuffers(PARTICLE_MESH_BUFFERS, _vertex_array_buffers);

	glBindBuffer(GL_ARRAY_BUFFER, _vertex_array_buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particle_model.positions[0]) * particle_model.positions.size(), 
		&particle_model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vertex_array_buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(particle_model.indices[0]) * particle_model.indices.size(), 
		&particle_model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);


	//glGenBuffers(1, &billboard_vertex_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// The VBO containing the positions and sizes of the particles
	glGenBuffers(1, &_particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, _particles_position_buffer);
	// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// The VBO containing the colors of the particles
	//GLuint particles_color_buffer;
	//glGenBuffers(1, &particles_color_buffer);
	//glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	//// Initialize with empty (NULL) buffer : it will be updated later, each frame.
	//glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
}


void ParticleSystem::horizontal_set()
{
	float dispersion = 10.f;

	for (unsigned int i = 0; i < MAX_PARTICLES; i++)
	{
		_particles[i].position = glm::vec2(
			dispersion * (float)std::rand() / (float)RAND_MAX - dispersion / 2.0f,
			0.0f);
		_particles[i].direction = glm::normalize(glm::vec2(
			0.0f,
			(float)std::rand() / (float)RAND_MAX - 0.5f
			));
		_particles[i].speed = 0.5f;
	}
}

void ParticleSystem::random_set()
{
	float dispersion = 10.f;

	for (unsigned int i = 0; i < MAX_PARTICLES; i++)
	{
		_particles[i].position = glm::vec2(
			dispersion * (float)std::rand() / (float)RAND_MAX - dispersion / 2.0f,
			dispersion * (float)std::rand() / (float)RAND_MAX - dispersion / 2.0f);
		_particles[i].direction = glm::normalize(glm::vec2(
			(float)std::rand() / (float)RAND_MAX - 0.5f,
			(float)std::rand() / (float)RAND_MAX - 0.5f
			));
		_particles[i].speed = 0.5f;
	}
}

void ParticleSystem::update_buffer()
{

	glBindBuffer(GL_ARRAY_BUFFER, _particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, _particles_count * 3 * sizeof(GLfloat), g_particule_position_size_data);

	//glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	//glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	//glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _particles_position_buffer);
	glVertexAttribPointer(
		1, // attribute. No particular reason for 1, but must match the layout in the shader.
		3, // size : x + y + size => 3
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
		);

}

void ParticleSystem::update_particles(float time_delta, float distortion_coefficient, const Camera2D& camera)
{
	_particles_count = 0;
	for (int i = 0; i < MAX_PARTICLES; i++){

		Particle& p = _particles[i]; // shortcut

		//if (p.life > 0.0f){

		//	// Decrease life
		//	p.life -= time_delta;
		//	if (p.life > 0.0f){

		float r = glm::length(p.position - camera.get_position());
		float r2 = pow(r, distortion_coefficient);
		
		float narrowed;

		if (r > 1.f)
			narrowed = r2 / (r2 + 1.0f);
		else
			narrowed = r / 2.0f;

		float speed = (1.0f - narrowed) * p.speed;

				// Simulate simple physics : gravity only, no collisions
				//p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
		p.position += p.direction * time_delta * speed;

		if (glm::abs(p.position[0]) > 10.f)
			p.direction[0] = -p.direction[0];

		if (glm::abs(p.position[1]) > 10.f)
			p.direction[1] = -p.direction[1];
	    //p.cameradistance = glm::length2(p.pos - CameraPosition);
				//ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

				// Fill the GPU buffer
		g_particule_position_size_data[3 * _particles_count + 0] = p.position.x;
		g_particule_position_size_data[3 * _particles_count + 1] = p.position.y;
		//g_particule_position_size_data[4 * ParticlesCount + 2] = p.position.z;

		g_particule_position_size_data[3 * _particles_count + 3] = p.size;

		//g_particule_color_data[4 * ParticlesCount + 0] = p.r;
		//g_particule_color_data[4 * ParticlesCount + 1] = p.g;
		//g_particule_color_data[4 * ParticlesCount + 2] = p.b;
		//g_particule_color_data[4 * ParticlesCount + 3] = p.a;

			//}
			//else{
			//	// Particles that just died will be put at the end of the buffer in SortParticles();
			//	p.cameradistance = -1.0f;
			//}

		_particles_count++;

		//}
	}
}

void ParticleSystem::draw()
{
	//glVertexAttribDivisor(2, 1); // color : one per quad -> 1

	glEnableVertexAttribArray(0);
	glBindVertexArray(_vertex_array_object);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _particles_position_buffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1
	// Draw the particules !
	// This draws many times a small triangle_strip (which looks like a quad).
	// This is equivalent to :
	// for(i in ParticlesCount) : glDrawArrays(GL_TRIANGLE_STRIP, 0, 4),
	// but faster.
	//std::cout << _particles_count << std::endl;
	glDrawElementsInstanced(GL_TRIANGLES, _num_of_mesh_indices, GL_UNSIGNED_INT, 0, _particles_count);
}

#endif