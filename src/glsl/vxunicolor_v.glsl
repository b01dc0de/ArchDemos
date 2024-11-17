#version 330

uniform mat4 MVP;
uniform vec4 uCol;

in vec3 vPos;
out vec4 color;

void main()
{
    gl_Position = MVP * vec4(vPos, 1.0);
    color = uCol;
};
