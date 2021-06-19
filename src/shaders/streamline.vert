#version 460 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in float aValue;
layout (location = 2) in float aWidth;

out VS_OUT {
    float rValue;
    float rWidth;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float value_max;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);

    vs_out.rValue = aValue / value_max;
    vs_out.rWidth = aWidth;
}