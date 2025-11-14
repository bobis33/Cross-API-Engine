#include <array>
#include <stdexcept>

#include "OPGL/OPGL.hpp"

void cae::OPGL::initialize(const NativeWindowHandle &nativeWindowHandle)
{
    if (eglBindAPI(EGL_OPENGL_API) == EGL_FALSE)
    {
        throw std::runtime_error("Failed to bind OpenGL API");
    }

    m_eglDisplay = eglGetDisplay(nativeWindowHandle.display);
    if (m_eglDisplay == EGL_NO_DISPLAY) {
        throw std::runtime_error("Failed to get EGL display");
}

    if (eglInitialize(m_eglDisplay, nullptr, nullptr) == EGL_FALSE) {
        throw std::runtime_error("Failed to initialize EGL");
}

    const EGLint configAttribs[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    if (eglChooseConfig(m_eglDisplay, configAttribs, &config, 1, &numConfigs) == EGL_FALSE || numConfigs == 0)
        throw std::runtime_error("Failed to choose EGL config");

    m_surface = eglCreateWindowSurface(m_eglDisplay, config, reinterpret_cast<EGLNativeWindowType>(nativeWindowHandle.window), nullptr);
    if (m_surface == EGL_NO_SURFACE)
        throw std::runtime_error("Failed to create EGL surface");

    constexpr EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE };
    m_context = eglCreateContext(m_eglDisplay, config, EGL_NO_CONTEXT, contextAttribs);
    if (m_context == EGL_NO_CONTEXT)
        throw std::runtime_error("Failed to create EGL context");

    if (eglMakeCurrent(m_eglDisplay, m_surface, m_surface, m_context) == EGL_FALSE)
        throw std::runtime_error("Failed to make EGL context current");

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(eglGetProcAddress)) == 0)
        throw std::runtime_error("Failed to initialize GLAD");

    glEnable(GL_DEPTH_TEST);
    glClearColor(1.f, 1.f, 1.f, 1.f);

    createShaderProgram();
    createTriangle();

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
    glViewport(0, 0, windowSize.width, windowSize.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(gShaderProgram);
    glBindVertexArray(gVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    eglSwapBuffers(m_eglDisplay, m_surface);
}
