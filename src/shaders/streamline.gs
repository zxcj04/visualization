#version 460 core
layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    float rWidth;
} gs_in[];

uniform float modifier;

void main()
{
    float width = gs_in[0].rWidth;

    vec4 line = gl_in[1].gl_Position - gl_in[0].gl_Position;

    vec3 direction = normalize(cross(line.xyz, vec3(0.0, 0.0, 1.0)));

    vec4 offset = vec4(direction, 0.0) * modifier * 0.005 * width;

    gl_Position = gl_in[0].gl_Position - offset / 2;

    EmitVertex();

    gl_Position = gl_in[1].gl_Position - offset / 2;

    EmitVertex();

    gl_Position = gl_in[0].gl_Position + offset / 2;

    EmitVertex();

    gl_Position = gl_in[1].gl_Position + offset / 2;

    EmitVertex();

    EndPrimitive();
}