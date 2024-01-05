#version 330 core

layout(location = 0) in vec3 _pos;

layout(location = 1) in vec3 _color;
out vec3 shapeColor;

layout(location = 2) in vec3 _normal;

layout(location = 3) in vec2 _texture2D;
out vec2 textureCoord;

out vec3 normal;
out vec3 crntPos;

uniform mat4 _mvp;
uniform mat4 _model;

void main()
{
    crntPos = vec3(_model * vec4(_pos, 1.0f));

    gl_Position = _mvp * _model * vec4(crntPos, 1.0);
    shapeColor = _color;
    textureCoord = _texture2D;

    normal = _normal;
};