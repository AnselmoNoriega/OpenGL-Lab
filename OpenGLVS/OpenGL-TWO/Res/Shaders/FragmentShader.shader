#version 330 core

out vec4 color;

in vec3 crntPos;
in vec3 shapeColor;
in vec3 normal;
in vec2 textureCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 _lightColor;
uniform vec3 _lightPos;
uniform vec3 _camPos;

float near = 0.1f;
float far = 100.0f;

vec4 PointLight()
{
    vec3 lightVec = _lightPos - crntPos;
    float dist = length(lightVec);
    float a = 0.05f;
    float b = 0.01f;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    float ambient = 0.2f;

    vec3 myNormal = normalize(normal);
    vec3 lightDir = normalize(lightVec);

    float diffuse = max(dot(myNormal, lightDir), 0.0f);

    float specularLight = 0.5f;
    vec3 viewDirection = normalize(_camPos - crntPos);
    vec3 reflectionDir = reflect(-lightDir, myNormal);
    float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, textureCoord) * (diffuse * inten + ambient) + texture(specular0, textureCoord).r * specular * inten) * _lightColor;
}

vec4 DirectLight()
{
    float ambient = 0.2f;

    vec3 myNormal = normalize(normal);
    vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f));

    float diffuse = max(dot(myNormal, lightDir), 0.0f);

    float specularLight = 0.5f;
    vec3 viewDirection = normalize(_camPos - crntPos);
    vec3 reflectionDir = reflect(-lightDir, myNormal);
    float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 16);
    float specular = specAmount * specularLight;

    return (texture(diffuse0, textureCoord) * (diffuse + ambient) + texture(specular0, textureCoord).r * specular) * _lightColor;
}

vec4 SpotLight()
{
    float outerCone = 0.9f;
    float innerCone = 1.0f;

    float ambient = 0.2f;

    vec3 myNormal = normalize(normal);
    vec3 lightDir = normalize(_lightPos - crntPos);

    float diffuse = max(dot(myNormal, lightDir), 0.0f);

    float specularLight = 0.5f;
    vec3 viewDirection = normalize(_camPos - crntPos);
    vec3 reflectionDir = reflect(-lightDir, myNormal);
    float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 16);
    float specular = specAmount * specularLight;

    float angle = dot(vec3(0.0f, 0.0f, -1.0f), - lightDir);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    return (texture(diffuse0, textureCoord) * (diffuse * inten + ambient) + texture(specular0, textureCoord).r * specular * inten) * _lightColor;
}

float LinearizeDepth(float depth)
{
    return (2.0 * near * far) / (far + near - (depth * 2.0 - 1.0) * (far - near));

}

float LogisticDepth(float depth, float steepness = 0.5f, float offset = 5.0f)
{
    float zVal = LinearizeDepth(depth);
    return (1 / (1 + exp(-steepness * (zVal - offset))));

}

void main()
{
    float depth = LogisticDepth(gl_FragCoord.z);
    color = DirectLight() * (1.0f - depth) + vec4(depth * vec3(0.85f, 0.85f, 0.90f), 1.0f);
}