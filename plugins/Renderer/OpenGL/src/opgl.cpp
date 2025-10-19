#include <array>
#include <iostream>
#include <stdexcept>

#include "OPGL/OPGL.hpp"

void cae::OPGL::initialize(const NativeWindowHandle &nativeWindowHandle)
{
#if defined(__linux__)
    m_display = static_cast<Display *>(nativeWindowHandle.display);
    m_window = reinterpret_cast<::Window>(nativeWindowHandle.window);

    if ((m_display == nullptr) || (m_window == 0U))
    {
        throw std::runtime_error("Invalid native window handle");
    }

    static int visualAttribs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo *vi = glXChooseVisual(m_display, DefaultScreen(m_display), visualAttribs);
    if (vi == nullptr)
    {
        throw std::runtime_error("No appropriate GLX visual found");
    }

    m_context = glXCreateContext(m_display, vi, nullptr, GL_TRUE);
    if (m_context == nullptr)
    {
        throw std::runtime_error("glXCreateContext failed");
    }

    if (glXMakeCurrent(m_display, m_window, m_context) == 0)
    {
        throw std::runtime_error("glXMakeCurrent failed");
    }

    if (gladLoadGLLoader((GLADloadproc)glXGetProcAddress) == 0)
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);

    createShaderProgram();
    createTriangle();

    std::cout << "[OPGL] OpenGL initialized\n";
#endif
}

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

    auto compileShader = [](GLenum type, const char *src) -> GLuint
    {
        const GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        GLint success;
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

    GLint success;
    glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &success);
    if (!success)
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
    constexpr std::array vertices = {-0.5f, -0.5f, 1.f,  0.f,  0.f, 0.5f, -0.5f, 0.f,
                                     1.f,   0.f,   0.0f, 0.5f, 0.f, 0.f,  1.f};

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

void cae::OPGL::draw(const WindowSize &windowSize)
{
#if defined(__linux__)
    glViewport(0, 0, windowSize.width, windowSize.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(gShaderProgram);
    glBindVertexArray(gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glXSwapBuffers(m_display, m_window);
#endif
}
