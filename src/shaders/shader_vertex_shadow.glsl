#version 330 core

layout (location = 0) in vec4 model_coefficients;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 shadowDir;
uniform float shadowGroundY;

void main()
{
    vec4 worldPos = model * model_coefficients;
    float t = (shadowGroundY - worldPos.y) / shadowDir.y;
    worldPos.xyz += shadowDir * t;
    gl_Position = projection * view * worldPos;
}