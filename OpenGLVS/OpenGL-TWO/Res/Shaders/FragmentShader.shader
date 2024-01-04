#version 330 core

out vec4 color;

in vec3 shapeColor;
in vec2 textureCoord;
in vec3 normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform vec4 _lightColor;
uniform vec3 _lightPos;

void main()
{
    float ambient = 0.2f;

    vec3 myNormal = normalize(normal);
    vec3 lightDir = normalize(_lightPos - crntPos);

    float diffuse = max(dot(myNormal, lightDir), 0.0f);

    color = texture(tex0, textureCoord) * _lightColor * (diffuse + ambient);
}