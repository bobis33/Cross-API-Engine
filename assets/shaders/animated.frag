#version 450 core

layout(location = 0) out vec4 FragColor;

layout(binding = 1) uniform TimeColor {
    float time;
} uTime;

void main()
{
    float r = 0.5 + 0.5 * sin(uTime.time);
    float g = 0.5 + 0.5 * sin(uTime.time * 1.3);
    float b = 0.5 + 0.5 * sin(uTime.time * 2.1);
    FragColor = vec4(r, g, b, 1.0);
}
