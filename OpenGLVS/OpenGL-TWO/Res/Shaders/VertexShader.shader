#version 330 core

layout(location = 0) in vec3 _pos;

layout(location = 1) in vec3 _color;
out vec3 shapeColor;

uniform float scale;

void main()
{
    gl_Position = vec4(_pos.x * scale, _pos.y * scale, _pos.z * scale, 1.0);
    shapeColor = _color;
};