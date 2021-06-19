#version 460 core

in float value;

out vec4 FragColor;

uniform vec3 view_pos;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform sampler1D using_texture1;

void main()
{
    vec4 color = texture(using_texture1, value);
    FragColor = color;
}