#version 450 core

layout(location = 0) in vec2 vUV;
layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
    // Même sans texture bindée, ça se compile.
    FragColor = texture(uTexture, vUV);
}
