#version 460 compatibility

in vec3 vN;
in vec3 v;
in vec4 p;

uniform bvec3 light_enable;
uniform sampler2D myTex;
uniform sampler2D myTex1;
uniform bool mix;

void main(void)
{
    vec3 N = normalize(vN);
    vec4 finalLight = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 texel = texture2D(myTex, gl_TexCoord[0].st * 2);
    vec4 texel1 = texture2D(myTex1, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t * 16));

    // finalLight += global_ambient;

    // ----------------------------------------------

    vec3 L0 = normalize(gl_LightSource[0].position.xyz);
    vec3 E0 = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 R0 = normalize(-reflect(L0,N));

    //calculate Ambient Term:
    vec4 Iamb0 = gl_FrontLightProduct[0].ambient;
    //calculate Diffuse Term:
    vec4 Idiff0 = gl_FrontLightProduct[0].diffuse * max(dot(N,L0), 0.0);
    Idiff0 = clamp(Idiff0, 0.0, 1.0);

    // calculate Specular Term:
    vec4 Ispec0 = gl_FrontLightProduct[0].specular
            * pow(max(dot(R0,E0),0.0),0.3*gl_FrontMaterial.shininess);
    Ispec0 = clamp(Ispec0, 0.0, 1.0);

    if(light_enable[0])
        finalLight += Idiff0 + Ispec0;

    // ----------------------------------------------

    vec3 L1 = normalize(gl_LightSource[1].position.xyz - v);
    vec3 E1 = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 R1 = normalize(-reflect(L1,N));

    //calculate Ambient Term:
    vec4 Iamb1 = gl_FrontLightProduct[1].ambient;
    //calculate Diffuse Term:
    vec4 Idiff1 = gl_FrontLightProduct[1].diffuse * max(dot(N,L1), 0.0);
    Idiff1 = clamp(Idiff1, 0.0, 1.0);

    // calculate Specular Term:
    vec4 Ispec1 = gl_FrontLightProduct[1].specular
            * pow(max(dot(R1,E1),0.0),0.3*gl_FrontMaterial.shininess);
    Ispec1 = clamp(Ispec1, 0.0, 1.0);

    if(light_enable[1])
        finalLight += Idiff1 + Ispec1;

    // ----------------------------------------------

    vec3 L2 = normalize(gl_LightSource[2].position.xyz - v);
    vec3 E2 = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)
    vec3 R2 = normalize(-reflect(L2,N));

    vec3 d = normalize(gl_LightSource[2].spotDirection.xyz);

    float cosb = -dot(L2, d)/(length(L2) * length(d));

    //calculate Ambient Term:
    vec4 Iamb2 = gl_FrontLightProduct[2].ambient;
    //calculate Diffuse Term:
    vec4 Idiff2 = gl_FrontLightProduct[2].diffuse * pow(cosb, gl_LightSource[2].spotExponent) * max(dot(N,L2), 0.0);
    Idiff2 = clamp(Idiff2, 0.0, 1.0);

    // calculate Specular Term:
    vec4 Ispec2 = gl_FrontLightProduct[2].specular * pow(cosb, gl_LightSource[2].spotExponent)
            * pow(max(dot(R2,E2),0.0),0.3*gl_FrontMaterial.shininess);
    Ispec2 = clamp(Ispec2, 0.0, 1.0);

    if(light_enable[2] && cos(radians(gl_LightSource[2].spotCutoff)) < cosb)
        finalLight += Idiff2 + Ispec2;

    finalLight += Iamb0 + Iamb1 + Iamb2;

    // ----------------------------------------------

    if(mix)
    {
        float py = p.y;

        if(py > 250)
            gl_FragColor = (gl_FrontLightModelProduct.sceneColor + finalLight) * texel1;
        else if(py > 50)
            gl_FragColor = (gl_FrontLightModelProduct.sceneColor + finalLight) * (texel * (1 - (py - 50)/200) + texel1 * (py - 50)/200);
        else
            gl_FragColor = (gl_FrontLightModelProduct.sceneColor + finalLight) * texel;
    }
    else
        gl_FragColor = (gl_FrontLightModelProduct.sceneColor + finalLight) * texel;
}