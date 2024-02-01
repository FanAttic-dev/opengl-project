#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

in vec3 position;
in vec3 normal;
in vec2 texture_coordinate;

out vec3 vert_pos;
out vec3 vert_normal;
out vec2 vert_tex_coord;

void main()
{
    vert_pos = vec3(model_matrix * vec4(position, 1.0));
    vert_normal = inverse(transpose(mat3(model_matrix))) * normal;
    vert_tex_coord = texture_coordinate;

    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);
}
