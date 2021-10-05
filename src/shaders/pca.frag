#version 460 core

out vec4 FragColor;

uniform vec3 view_pos;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform sampler1D using_texture1;

in float point_class;

void main()
{
    vec4 color = texture(using_texture1, point_class);
    FragColor = color;
}