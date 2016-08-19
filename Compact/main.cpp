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
#include "shader.h"
#include "transform_2d.h"

#undef main_

static const int DISPLAY_WIDTH = 1000;
static const int DISPLAY_HEIGHT = 600;

static const float MAX_TIME_PER_FRAME = 1000.f / 45.f;

static const float MAP_SIZE = 10;
static const unsigned int TICKS = 100;

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

	BasicShader basic_shader("basicShader");
	DeformationShader compact_shader("compact_shader");
	Camera2D camera(glm::vec2(), (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.01f);

	unsigned int time = 0;

	bool fullScreen = false;
	while (isRunning)
	{
		//camera.set_position(glm::vec2((float)MAP_SIZE * glm::sin(glm::pi<float>()/720.f*(float) time++), 0.0f));

		display.clear();

		//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		//glDisableVertexAttribArray(0);

		//basic_shader.bind();
		//basic_shader.update(Transform2D(), camera, BLACK);
		compact_shader.bind();
		compact_shader.update(Transform2D(), camera, BLACK);
		grid_mesh.drawLines();
		//grid_mesh.drawPoints();

		//glPointSize(5.0f);
		//glColor3f(0.0f, 0.0f, 0.0f);
		//glBegin(GL_POINTS); //starts drawing of points
		//glVertex2f(1.0f, 1.0f);//upper-right corner
		//glVertex2f(-1.0f, -1.0f);//lower-left corner
		//glEnd();//end drawing of points
		//glPointSize(1.0f);

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

		//SDL_Delay(1000 / 60);

		//clock_t end = clock();
		//float elapsed_millis = (1000.0f * (end - begin) / CLOCKS_PER_SEC);
		//begin = end;
		//if (elapsed_millis < MAX_TIME_PER_FRAME)
		//	SDL_Delay((unsigned)(MAX_TIME_PER_FRAME - elapsed_millis));
	}

	return 0;
}
