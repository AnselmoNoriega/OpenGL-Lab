#version 330 core

layout(location = 0) in vec3 _pos;

uniform mat4 lightProj;
uniform mat4 _model;

void main()
{
	gl_Position = lightProj * _model * vec4(_pos, 1.0f);
};