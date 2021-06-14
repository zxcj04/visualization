#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in float aWidth;

out VS_OUT {
    float rWidth;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

    vs_out.rWidth = aWidth;
}