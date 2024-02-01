#version 330

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

in vec3 position;

out vec3 vert_tex_coord;

void main()
{
    vert_tex_coord = position;
    vec4 pos = projection_matrix * view_matrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}
