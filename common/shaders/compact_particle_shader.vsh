#version 330 core

// _m - modelspace
// _w - worldspace
// _c - cameraspace

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec3 particle_position;

out float nr;
out float r;

uniform mat3 MV;
uniform mat4 Projection;
uniform float Coefficient;

void main()
{
	vec2 position_c = vec2(MV * vec3(vertex_position + particle_position.xy, 1.0f));

	r = length(position_c);
	float r2 = pow(r, Coefficient);
	// float r2 = r * r;

	// float narrowed = r2 / (r2 + 1.0f);

	float narrowed;

	if (r > 1.f)
		narrowed = r2 / (r2 + 1.0f);
	else
		narrowed = r / 2.0f;

	nr = narrowed / r;

	//gl_Position = vec4(position_c.x * narrowed / r, position_c.y * narrowed / r, 0.0f, 1.0f);
	gl_Position = Projection * vec4(position_c.x * nr, position_c.y * nr, 0.0f, 1.0f);
	//gl_Position = Projection * vec4(position_c, 0.0f, 1.0f);
}
