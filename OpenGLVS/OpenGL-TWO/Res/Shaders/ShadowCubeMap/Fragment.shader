#version 330 core

in vec4 FragPos;

uniform vec3 _lightPos;
uniform float _farPlane;

void main()
{
	gl_FragDepth = length(FragPos.xyz - _lightPos) / _farPlane;
};