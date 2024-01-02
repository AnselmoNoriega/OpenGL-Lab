#version 330 core

out vec4 color;

in vec3 shapeColor;
in vec2 textureCoord;

uniform sampler2D tex0;

void main()
{
    color = texture(tex0, textureCoord);
}