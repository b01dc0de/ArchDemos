#version 330

uniform mat4 MVP;
uniform vec3 uCol;

in vec3 vPos;
out vec3 color;

void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    color = uCol;
};
