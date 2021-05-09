#version 460 core

out vec4 FragColor;

in vec3 frag_pos;
in vec3 tex_coord;

uniform vec3 view_pos;
uniform vec3 light_pos;
uniform vec3 light_color;
uniform bool enable_section;
uniform vec3 base_color;

uniform sampler3D using_texture1;

void main()
{
    vec4 texture_color = texture(using_texture1, tex_coord);

    FragColor = texture_color;
    return;

	// vec3 base_color = vec3(0.7, 0.5, 0.5);

	// FragColor = vec4(our_color, 1.0);
	// FragColor = vec4(gradient, 1.0);
	// return;

    vec3 gradient = vec3(0, 1, 0);

    vec3 norm = normalize(gradient);

    vec3 light_direction = normalize(light_pos - frag_pos);
    vec3 view_direction = normalize(view_pos - frag_pos);

    if (dot(norm, -view_direction) < 0)
	{
        norm = -norm;
    }

    vec3 reflect_direction = reflect(-light_direction, norm);

    float ambient_strength = 0.2;
    vec3 ambient = ambient_strength * light_color;
    // vec3 ambient = vec3(0, 0, 0);

    float diff = max(dot(norm, light_direction), 0.0);
    float diff_strength = 0.7;
    vec3 diffuse = diff_strength * diff * light_color;
    // vec3 diffuse = vec3(0, 0, 0);

    float specular_strength = 0.5;
    vec3 specular = specular_strength * pow(max(dot(view_direction, reflect_direction), 0.0), 32) * light_color;
    // vec3 specular = vec3(0, 0, 0);

    vec3 light = clamp((ambient + diffuse + specular), vec3(0.0), vec3(1.0)) * base_color;
    FragColor = vec4(light, 1.0);

	vec3 tmp = vec3(1.0, 0.0, 0.0);
	FragColor = vec4(tmp, 1.0);
}