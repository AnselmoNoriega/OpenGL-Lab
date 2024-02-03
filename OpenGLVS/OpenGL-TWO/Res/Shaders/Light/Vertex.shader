#version 330 core

layout(location = 0) in vec3 _pos;

uniform mat4 _mvp;
uniform mat4 _model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
    vec3 crntPos = vec3(_model * translation * rotation * scale * vec4(_pos, 1.0f));
    gl_Position = _mvp * vec4(crntPos, 1.0);
};