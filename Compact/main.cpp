#include <algorithm>
#include <chrono>
#include <iostream>
#include <math.h>
#include <ratio>
#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "camera_2d.h"
#include "display.h"
#include "mesh.h"
#include "mesh_creator.h"
#include "particle.h"
#include "shader.h"
#include "transform_2d.h"

#undef main_

static const int DISPLAY_WIDTH = 1000;
static const int DISPLAY_HEIGHT = 600;

static const float MAX_TIME_PER_FRAME = 1000.f / 45.f;

static const float MAP_SIZE = 10;
static const unsigned int TICKS = 200;

static const glm::vec3 BLACK = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 GRAY = glm::vec3(0.5f, 0.5f, 0.5f);
static const glm::vec3 WHITE = glm::vec3(1.0f, 1.0f, 1.0f);

int main(int argc, char **argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Local");
	display.clear(.2f, .4f, 0.7f, 1.0f);

	bool isRunning = true;
	SDL_Event e;

	const Uint8* state = SDL_GetKeyboardState(NULL);
	clock_t begin = clock();

	LineMesh grid_mesh(MeshCreator::sqare_grid(TICKS, TICKS, MAP_SIZE, MAP_SIZE));
	LineMesh circle(MeshCreator::circle_full_triangles(16, .1f));

	BasicShader basic_shader("basicShader");
	BasicShader basic_particle_shader("basic_particle_shader");
	DeformationShader compact_shader("compact_shader");
	DeformationShader sh_compact_particle("compact_particle_shader");
	float deformation = 2.0f;

	Camera2D camera(glm::vec2(), (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.01f);

	ParticleSystem particles(MeshCreator::circle_full_triangles(8, 0.05f));

	unsigned int time = 0;

	//particles.update_particles(0.017f);
	//particles.update_buffer();

	bool fullScreen = false;
	while (isRunning)
	{
		//camera.set_position(glm::vec2((float)MAP_SIZE * glm::sin(glm::pi<float>()/720.f*(float) time++), 0.0f));

		display.clear();


		//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		//glDisableVertexAttribArray(0);

		//basic_shader.bind();
		//basic_shader.update(Transform2D(), camera, BLACK);
		//circle.draw<GL_TRIANGLES>();

		compact_shader.bind();
		compact_shader.update(Transform2D(), camera, BLACK, deformation);
		grid_mesh.draw<GL_LINES>();

		particles.update_particles(0.017f, deformation, camera);
		particles.update_buffer();

		sh_compact_particle.bind();
		sh_compact_particle.update(Transform2D(), camera, GRAY, deformation);
		//basic_particle_shader.bind();
		//basic_particle_shader.update(Transform2D(), camera, GRAY);
		particles.draw();

		display.swapBuffers();

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				isRunning = false;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_l:
					display.toggleFullScreen();
					// scene->setAspectRatio(display.getAspectRatio());
					fullScreen = !fullScreen;
					break;
				case SDLK_r:
					particles.random_set();
					break;
				case SDLK_h:
					particles.horizontal_set();
					break;
				}
			}
		}

		glm::vec2 movement;
		bool is_movement = false;

		if (state[SDL_SCANCODE_UP]) {
			movement += glm::vec2(0.0f, 1.0f);
			is_movement = true;
		}
		else if (state[SDL_SCANCODE_DOWN])
		{
			movement += glm::vec2(0.0f, -1.0f);
			is_movement = true;
		}
		
		if (state[SDL_SCANCODE_RIGHT])
		{
			movement += glm::vec2(1.0f, 0.0f);
			is_movement = true;
		}
		else if (state[SDL_SCANCODE_LEFT])
		{
			movement += glm::vec2(-1.0f, 0.0f);
			is_movement = true;
		}
		if (is_movement)
		{
			camera.move(glm::normalize(movement));
		}

		if (state[SDL_SCANCODE_Q]) {
			deformation = std::min(deformation + 0.05f, 10.0f);
		}
		else if (state[SDL_SCANCODE_A])
		{
			deformation = std::max(deformation - 0.05f, 0.2f);
		}
		else if (state[SDL_SCANCODE_Z])
		{
			deformation = 2.0f;
		}

		SDL_Delay(1000 / 60);

		//clock_t end = clock();
		//float elapsed_millis = (1000.0f * (end - begin) / CLOCKS_PER_SEC);
		//begin = end;
		//if (elapsed_millis < MAX_TIME_PER_FRAME)
		//	SDL_Delay((unsigned)(MAX_TIME_PER_FRAME - elapsed_millis));
	}

	return 0;
}
