#version 330 core

layout(location = 0) in vec3 _pos;

layout(location = 1) in vec3 _color;
out vec3 shapeColor;

layout(location = 2) in vec2 _texture2D;
out vec2 textureCoord;

uniform mat4 _mvp;

void main()
{
    gl_Position = _mvp * vec4(_pos, 1.0);
    shapeColor = _color;
    textureCoord = _texture2D;
};