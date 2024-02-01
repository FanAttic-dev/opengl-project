#version 330

uniform samplerCube skybox;
uniform vec3 eye_pos;

in vec3 vert_pos;
in vec3 vert_normal;

out vec4 final_color;

void main()
{
    vec3 I = normalize(vert_pos - eye_pos);
    vec3 R = reflect(I, normalize(vert_normal));
    final_color = vec4(texture(skybox, R).rgb, 1.0);
}
