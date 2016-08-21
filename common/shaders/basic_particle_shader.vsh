#version 330 core

// _m - modelspace
// _w - worldspace
// _c - cameraspace

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec3 particle_position;

out vec4 position_w;

uniform mat4 MVP;
uniform mat4 Model;

void main()
{
	gl_Position = MVP * vec4(particle_position.xy + vertex_position, 0.0f, 1.0f);
}
