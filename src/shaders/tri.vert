#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aGradient;

out vec3 gradient;
out vec3 frag_pos;
out float clipped;

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

    clipped = dot(vec4(frag_pos, 1.0), clip);

	// gradient = aGradient;
    gradient = mat3(transpose(inverse(model))) * aGradient;
}