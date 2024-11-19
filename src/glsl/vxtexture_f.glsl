#version 330

uniform sampler2D Texture0;
in vec2 uv;
out vec4 fragment;

void main()
{
	fragment = texture(Texture0, uv);
}

