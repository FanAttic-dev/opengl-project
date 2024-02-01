#version 330

uniform samplerCube skybox;

in vec3 vert_tex_coord;

out vec4 final_color;

void main()
{
    final_color = texture(skybox, vert_tex_coord);
}
