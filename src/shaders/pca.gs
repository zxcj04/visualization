#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
    float rClass;
} gs_in[];

out float point_class;

void main()
{
    point_class = gs_in[0].rClass;

    float modifier = 0.0075;

    gl_Position = gl_in[0].gl_Position + vec4(-1.0, -1.0, 0.0, 0.0) * modifier;

    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(1.0, -1.0, 0.0, 0.0) * modifier;

    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(-1.0, 1.0, 0.0, 0.0) * modifier;

    EmitVertex();

    gl_Position = gl_in[0].gl_Position + vec4(1.0, 1.0, 0.0, 0.0) * modifier;

    EmitVertex();

    EndPrimitive();
}