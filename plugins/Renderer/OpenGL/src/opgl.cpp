#include <array>
#include <memory>
#include <stdexcept>

#include "OPGL/OPGL.hpp"

#if defined(__linux__)
#include "OPGL/Context/EGLContextLinux.hpp"
#elif defined(_WIN32)
#include "OPGL/Context/WGLContextWindows.hpp"
#elif defined(__APPLE__)
#include "OPGL/Context/NSGLContextMac.hpp"
#endif

void cae::OPGL::initialize(const NativeWindowHandle &window)
{
#if defined(__linux__)
    m_context = std::make_unique<EGLContextLinux>();
#elif defined(_WIN32)
    m_context = std::make_unique<WGLContextWindows>();
#elif defined(__APPLE__)
    m_context = std::make_unique<NSGLContextMac>();
#else
    static_assert(false, "Unsupported platform");
#endif
    m_context->initialize(window);

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.F, 1.F, 1.F, 1.F);

    createShaderProgram();
    createTriangle();
}

void cae::OPGL::draw(const WindowSize &windowSize)
{
    glViewport(0, 0, windowSize.width, windowSize.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gShaderProgram);
    glBindVertexArray(gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    m_context->swapBuffers();
}

void cae::OPGL::setVSyncEnabled(const bool enabled) { m_context->setVSyncEnabled(enabled); }

bool cae::OPGL::isVSyncEnabled() const { return m_context->isVSyncEnabled(); }

void cae::OPGL::createShaderProgram()
{
    const auto *const vertexShaderSource = R"(#version 330 core
        layout(location=0) in vec2 aPos;
        layout(location=1) in vec3 aColor;
        out vec3 ourColor;
        void main() { gl_Position = vec4(aPos,0.0,1.0); ourColor = aColor; })";

    const auto *const fragmentShaderSource = R"(#version 330 core
        in vec3 ourColor;
        out vec4 FragColor;
        void main() { FragColor = vec4(ourColor,1.0); })";

    auto compileShader = [](const GLenum type, const char *src) -> GLuint
    {
        const GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            throw std::runtime_error(log);
        }
        return shader;
    };

    const GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    const GLuint fragment = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    gShaderProgram = glCreateProgram();
    glAttachShader(gShaderProgram, vertex);
    glAttachShader(gShaderProgram, fragment);
    glLinkProgram(gShaderProgram);

    GLint success = 0;
    glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        char log[512];
        glGetProgramInfoLog(gShaderProgram, 512, nullptr, log);
        throw std::runtime_error(log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void cae::OPGL::createTriangle()
{
    constexpr std::array vertices = {-0.5F, -0.5F, 1.F,  0.F,  0.F, 0.5F, -0.5F, 0.F,
                                     1.F,   0.F,   0.0F, 0.5F, 0.F, 0.F,  1.F};

    glGenVertexArrays(1, &gVAO);
    glGenBuffers(1, &gVBO);

    glBindVertexArray(gVAO);
    glBindBuffer(GL_ARRAY_BUFFER, gVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
