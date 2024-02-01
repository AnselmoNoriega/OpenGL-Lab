#version 330 core

layout(location = 0) in vec2 _pos;
layout(location = 1) in vec2 _texCoords;

out vec2 textureCoord;

void main()
{
    gl_Position = vec4(_pos.x, _pos.y, 0.0, 1.0);
    textureCoord = _texCoords;
};