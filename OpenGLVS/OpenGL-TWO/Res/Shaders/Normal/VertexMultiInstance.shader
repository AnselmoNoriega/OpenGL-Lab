#version 330 core

layout(location = 0) in vec3 _pos; 
out vec3 crntPos;

layout(location = 1) in vec3 _color; 
out vec3 shapeColor;

layout(location = 2) in vec3 _normal; 
out vec3 normal;

layout(location = 3) in vec2 _texture2D; 
out vec2 textureCoord;

layout(location = 4) in mat4 _instanceMatrix;

uniform mat4 _mvp;


void main()
{
    crntPos = vec3(_instanceMatrix * vec4(_pos, 1.0f));
    shapeColor = _color;
    normal = _normal;
    textureCoord = mat2(0.0, -1.0, 1.0, 0.0) * _texture2D;

    gl_Position = _mvp * vec4(crntPos, 1.0);
};