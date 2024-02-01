#version 330 core 

layout (location = 0) in vec3 _pos;
out vec3 texCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vec4 pos = projection * view * vec4(_pos, 1.0f);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    texCoord = vec3(_pos.x, _pos.y, -_pos.z);

}