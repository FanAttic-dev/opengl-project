#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

in vec3 position;
in vec3 normal;

out vec3 vert_pos;
out vec3 vert_normal;

void main()
{
    vert_normal = mat3(transpose(inverse(model_matrix))) * normal;
    vert_pos = vec3(model_matrix * vec4(position, 1.0));
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);
}
