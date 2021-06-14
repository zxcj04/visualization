#version 460 core

out vec4 FragColor;

uniform vec3 view_pos;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 base_color;

void main()
{
    FragColor = vec4(base_color, 1.0);
}