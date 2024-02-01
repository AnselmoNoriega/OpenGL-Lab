#version 330 core

out vec4 FragColor;

uniform vec4 _lightColor;

void main()
{
	FragColor = _lightColor;
};