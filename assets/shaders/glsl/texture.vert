#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

layout(location = 0) out vec3 vColor;

// Uniforms must be in a block
layout(set = 0, binding = 0) uniform Matrices
{
    mat4 uMVP;
};

void main()
{
    vColor = aColor;
    gl_Position = uMVP * vec4(aPos, 1.0);
}
