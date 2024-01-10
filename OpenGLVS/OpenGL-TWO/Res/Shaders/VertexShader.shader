#version 330 core

layout(location = 0) in vec3 _pos;
out vec3 crntPos;

layout(location = 1) in vec3 _color;
out vec3 shapeColor;

layout(location = 2) in vec3 _normal;
out vec3 normal;

layout(location = 3) in vec2 _texture2D;
out vec2 textureCoord;

uniform mat4 _mvp;
uniform mat4 _model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
    crntPos = vec3(_model * translation * -rotation * scale * vec4(_pos, 1.0f));
    shapeColor = _color;
    normal = _normal;
    textureCoord = mat2(0.0, -1.0, 1.0, 0.0) * _texture2D;

    gl_Position = _mvp * vec4(crntPos, 1.0);
};