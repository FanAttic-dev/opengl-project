#version 330

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cut_off;
    float outer_cut_off;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2
#define NR_SPOT_LIGHTS 5

uniform vec3 eye_pos;
uniform Material material;
uniform DirLight dir_light;
uniform PointLight point_lights[NR_POINT_LIGHTS];
uniform SpotLight spot_lights[NR_SPOT_LIGHTS];

in vec3 vert_pos;
in vec3 vert_normal;
in vec2 vert_tex_coord;

out vec4 final_color;

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 vert_pos, vec3 view_dir);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 vert_pos, vec3 view_dir);

void main()
{
    // properties
    vec3 normal = normalize(vert_normal);
    vec3 view_dir = normalize(eye_pos - vert_pos);

    vec3 result;

    // directional lighting
    result = calc_dir_light(dir_light, normal, view_dir);

    // point lights
    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        result += calc_point_light(point_lights[i], normal, vert_pos, view_dir);
    }

    // spot lights
    for (int i = 0; i < NR_SPOT_LIGHTS; ++i) {
        result += calc_spot_light(spot_lights[i], normal, vert_pos, view_dir);
    }

    final_color = vec4(result, texture(material.diffuse, vert_tex_coord).w);
}

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir) {
    vec3 light_dir = normalize(-light.direction); // to point towards the light source

    // diffuse
    float diff = max(dot(normal, light_dir), 0.0);

    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vert_tex_coord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vert_tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vert_tex_coord));

    return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 vert_pos, vec3 view_dir) {
    vec3 light_dir = normalize(light.position - vert_pos); // point towards the light source

    // diffuse
    float diff = max(dot(normal, light_dir), 0.0);

    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - vert_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vert_tex_coord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vert_tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vert_tex_coord));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 vert_pos, vec3 view_dir) {
    vec3 light_dir = normalize(light.position - vert_pos); // point towards the light source

    // diffuse
    float diff = max(dot(normal, light_dir), 0.0);

    // specular
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - vert_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spotlight intensity
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, vert_tex_coord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, vert_tex_coord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, vert_tex_coord));

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}
