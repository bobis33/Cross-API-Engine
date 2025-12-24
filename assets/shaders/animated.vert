#version 450 core

layout(location = 0) in vec2 aPos;

layout(binding = 0) uniform TimeData {
    float time;
} uTime;

void main()
{
    float scale = 0.5 + 0.5 * sin(uTime.time);
    gl_Position = vec4(aPos * scale, 0.0, 1.0);
}
