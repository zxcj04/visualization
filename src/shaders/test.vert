#version 460 compatibility

out vec3 vN;
out vec3 v;
out vec4 p;

void main(void)
{
    v = vec3(gl_ModelViewMatrix * gl_Vertex);

    vN = normalize(gl_NormalMatrix * gl_Normal);
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    gl_TexCoord[0] = gl_MultiTexCoord0;
    p = gl_Vertex;
}