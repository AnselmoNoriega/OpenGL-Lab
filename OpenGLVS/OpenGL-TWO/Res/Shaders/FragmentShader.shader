#version 330 core

out vec4 color;

in vec3 shapeColor;

void main()
{
    color = vec4(shapeColor, 1.0f);
}