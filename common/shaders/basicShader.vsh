#version 330 core

// _m - modelspace
// _w - worldspace
// _c - cameraspace

layout(location = 0) in vec2 position_m;

out vec4 position_w;

uniform mat4 MVP;
uniform mat4 Model;

void main()
{
	gl_Position = MVP * vec4(position_m, 0.0f, 1.0f);
	// color0 = color;
	// position_w = (Model * vec4(position_m, 1.0)).xyz;
}
