#version 330 core

out
vec4 color;

in vec3 crntPos;
in vec3 shapeColor;
in vec3 normal;
in vec2 textureCoord;
in vec4 fragPosLight;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D shadowMap;
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

    float specular = 0.0f;
    if(diffuse != 0.0f)
    {
        float specularLight = 0.5f;
        vec3 viewDirection = normalize(_camPos - crntPos);
        vec3 reflectionDir = reflect(-lightDir, myNormal);

        vec3 halfwayVec = normalize(viewDirection + lightDir);

        float specAmount = pow(max(dot(myNormal, halfwayVec), 0.0f), 16);
        specular = specAmount * specularLight;
    };

    return (texture(diffuse0, textureCoord) * (diffuse * inten + ambient) + texture(specular0, textureCoord).r * specular * inten) * _lightColor;
}

vec4 DirectLight()
{
    if (texture(diffuse0, textureCoord).a < 0.1)
    {
        discard;
    }
    
    float ambient = 0.2f;

    vec3 myNormal = normalize(normal);
    vec3 lightDir = normalize(vec3(1.0f, 1.0f, 0.0f));
    float diffuse = max(abs(dot(myNormal, lightDir)), 0.0f);

    float specularLight = 0.5f;
    vec3 viewDirection = normalize(_camPos - crntPos);
    vec3 reflectionDir = reflect(-lightDir, myNormal);

    float specAmount = pow(max(dot(viewDirection, reflectionDir), 0.0f), 16);
    float specular = specAmount * specularLight;

    float shadow = 0.0f;
    vec3 lightCoords = fragPosLight.xyz / fragPosLight.w;
    if(lightCoords.z <= 1.0f)
    {
        lightCoords = (lightCoords + 1.0f) / 2.0f;
        float currentDepth = lightCoords.z;
        float bias = max(0.25f * (1.0f - dot(myNormal, lightDir)), 0.005f);

        int sampleRadius = 2;
        vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
        for(int x = -sampleRadius; x <= sampleRadius; ++x)
        {
            for(int y = -sampleRadius; y <= sampleRadius; ++y)
            {
                float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
                if(currentDepth > closestDepth + bias)
                {
                    shadow += 1.0f;
                }
            }
        }

        shadow /= pow((sampleRadius * 2 + 1), 2);
    }
    
    return (texture(diffuse0, textureCoord) * (diffuse * (1.0f - shadow) + ambient) + texture(specular0, textureCoord).r * specular) * _lightColor;
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

    float angle = dot(vec3(0.0f, 0.0f, -1.0f), -lightDir);
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

vec4 FogEffect()
{
    float depth = LogisticDepth(gl_FragCoord.z);
    return DirectLight() * (1.0f - depth) + vec4(depth * vec3(0.1f, 0.1f, 0.4f), 1.0f);
}

void main()
{
    color = DirectLight();

}