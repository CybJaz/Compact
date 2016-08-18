#include <algorithm>
#include <chrono>
#include <iostream>
#include <math.h>
#include <ratio>
#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "display.h"
#include "mesh.h"
#include "mesh_creator.h"
//#include "shader.h"
//#include "transform.h"

#undef main_

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;

static const float MAX_TIME_PER_FRAME = 1000.f / 45.f;

static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
};

int main(int argc, char **argv)
{
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "Local");
	display.clear(.2f, .4f, 0.7f, 1.0f);

	bool isRunning = true;
	SDL_Event e;

	const Uint8* state = SDL_GetKeyboardState(NULL);
	clock_t begin = clock();

	LineMesh grid_mesh(MeshCreator::sqare_grid(4, 4, 0.5f, 0.5f));


	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	bool fullScreen = false;
	while (isRunning)
	{
		display.clear();

		//glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		//glDisableVertexAttribArray(0);

		grid_mesh.drawLines();

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

		SDL_Delay(1000 / 60);

		//clock_t end = clock();
		//float elapsed_millis = (1000.0f * (end - begin) / CLOCKS_PER_SEC);
		//begin = end;
		//if (elapsed_millis < MAX_TIME_PER_FRAME)
		//	SDL_Delay((unsigned)(MAX_TIME_PER_FRAME - elapsed_millis));
	}



	return 0;
}
