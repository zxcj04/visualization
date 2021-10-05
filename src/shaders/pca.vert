#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aClass;

out VS_OUT {
    float rClass;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float value_max;

void main()
{
	vs_out.rClass = aClass;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}