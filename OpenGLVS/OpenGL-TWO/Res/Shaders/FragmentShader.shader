#version 330 core

out vec4 color;

in vec3 shapeColor;
in vec2 textureCoord;
in vec3 normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform vec4 _lightColor;
uniform vec3 _lightPos;
uniform vec3 _camPos;

void main()
{
    float ambient = 0.2f;

    vec3 myNormal = normalize(normal);
    vec3 lightDir = normalize(_lightPos - crntPos);

    float diffuse = max(dot(myNormal, lightDir), 0.0f);

    float specularLight = 0.5f;
    vec3 viewDirection = normalize(_camPos - crntPos);
    vec3 reflectionDir = reflect(-lightDir, myNormal);
    float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 8);
    float specular = specAmount * specularLight;

    color = texture(tex0, textureCoord) * _lightColor * (diffuse + ambient + specular);
}