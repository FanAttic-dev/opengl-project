#version 330

uniform vec3 bulb_color;

out vec4 final_color;

void main()
{
    final_color = vec4(bulb_color, 1.0);
}
