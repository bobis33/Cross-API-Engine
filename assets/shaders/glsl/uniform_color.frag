#version 450 core

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform ColorData {
    vec3 color;
} uColor;

void main()
{
    FragColor = vec4(uColor.color, 1.0);
}
