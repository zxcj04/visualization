#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aTexCoord;

out vec3 frag_pos;
out vec3 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec4 clip;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	// gl_Position = vec4(aPos, 1.0f);
    frag_pos = vec3(model * vec4(aPos, 1.0));
	// frag_pos = aPos;

	tex_coord = aTexCoord;
}