#version 460 core

out vec4 FragColor;

in vec3 frag_pos;
in vec3 ori_pos;
in vec3 normal;

uniform vec3 view_pos;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform bool enable_section;
uniform vec3 resolution;
uniform vec3 voxelsize;
uniform bool shade;
uniform float gap;
uniform float last_decay;
uniform float modifier;

uniform sampler3D using_texture1;
uniform sampler1D using_texture2;

bool inside(vec3 pos)
{
    if((pos.x >= 0 && pos.x <= resolution.x) &&
       (pos.y >= 0 && pos.y <= resolution.y) &&
       (pos.z >= 0 && pos.z <= resolution.z))
        return true;
    else
        return false;
}

vec4 phong_shading(vec4 color, vec3 position, vec3 gradient)
{
    vec3 norm = normalize(gradient);

    vec3 light_direction = normalize(light_pos - position);
    vec3 view_direction = normalize(view_pos - position);

    if (dot(norm, -view_direction) < 0)
	{
        norm = -norm;
    }

    vec3 reflect_direction = reflect(-light_direction, norm);

    float ambient_strength = 0.2;
    vec3 ambient = ambient_strength * light_color;
    // ambient = vec3(0, 0, 0);

    float diff = max(dot(norm, light_direction), 0.0);
    float diff_strength = 0.7;
    vec3 diffuse = diff_strength * diff * light_color;
    // diffuse = vec3(0, 0, 0);

    float specular_strength = 0.5;
    vec3 specular = specular_strength * pow(max(dot(view_direction, reflect_direction), 0.0), 32) * light_color;
    // specular = vec3(0, 0, 0);

    vec3 light = clamp((ambient + diffuse + specular), vec3(0.0), vec3(1.0)) * color.rgb;
    return vec4(light, color.a);
}

void main()
{
    FragColor = vec4(0.0, 0.0, 0.0, 0.0);

    vec3 now_pos = ori_pos;
    vec3 now_frag_pos = frag_pos;
    float now_decay = 0.0;

    // float gap = 1;

    while(now_decay < last_decay && inside( now_pos ))
    {
        vec3 sample_point_tex_coord = now_pos / resolution;

        vec4 sample_point_3d_tex = texture(using_texture1, sample_point_tex_coord);

        vec4 sample_point_tex_color = texture(using_texture2, sample_point_3d_tex.a);

        now_pos += normalize(-view_pos / voxelsize) * gap;
        now_frag_pos += normalize(-view_pos) * gap;

        if((sample_point_tex_coord.x < 0 || sample_point_tex_coord.x > 1) ||
           (sample_point_tex_coord.y < 0 || sample_point_tex_coord.y > 1) ||
           (sample_point_tex_coord.z < 0 || sample_point_tex_coord.z > 1))
        {
            continue;
        }

        now_decay += sample_point_tex_color.a * (1 - now_decay);

        vec3 tmp = sample_point_tex_color.rgb;

        tmp *= sample_point_tex_color.a;

        sample_point_tex_color.rgb = tmp;

        sample_point_tex_color *= modifier;

        if(shade)
        {
            vec4 point_color = phong_shading(sample_point_tex_color, now_frag_pos, sample_point_tex_coord.rgb);

            FragColor += point_color;
        }
        else
            FragColor += sample_point_tex_color;
    }
}