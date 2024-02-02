#version 330 core

layout(location = 0) in vec3 _pos;

uniform mat4 lightProj;
uniform mat4 model;

void main()
{
	gl_Position = lightProj * model * vec4(_pos, 1.0f);
};