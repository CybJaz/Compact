#version 330 core

uniform vec3 Color;

in float nr;
in float r;

void main()
{
	gl_FragColor = vec4(Color, 1.0f);
	
	if (r < 1)
		gl_FragColor = vec4(Color, 1.0f);
	else if (r > 8)
		gl_FragColor = vec4(Color, 0.0f);
	else
		gl_FragColor = vec4(Color, 2.0f * nr);
	//else 
		//gl_FragColor = vec4( 0.f , 0.0f, 0.0f, 0.0f);
}
