#include <array>
#include <memory>
#include <stdexcept>

#include "OPGL/OPGL.hpp"
#include "Utils/Utils.hpp"
#ifdef __linux__
#include "OPGL/Context/EGLContextLinux.hpp"
#elifdef _WIN32
#include "OPGL/Context/WGLContextWindows.hpp"
#elifdef __APPLE__
#include "OPGL/Context/NSGLContextMac.hpp"
#endif

void cae::OPGL::initialize(const NativeWindowHandle &window, const std::shared_ptr<IShader> shader)
{
    m_shader = shader;
#ifdef __linux__
    m_context = std::make_unique<EGLContextLinux>();
#elifdef _WIN32
    m_context = std::make_unique<WGLContextWindows>();
#elifdef __APPLE__
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
    m_shader->addShader(
        "vertex",
        utl::fileToString("assets/shaders/uniform_color.vert"),
        ShaderStage::VERTEX
    );

    m_shader->addShader(
        "fragment",
        utl::fileToString("assets/shaders/uniform_color.frag"),
        ShaderStage::FRAGMENT
    );

    if (!m_shader->compileAll()) {
        throw std::runtime_error("Failed to compile shaders");
}

    const auto& vertSPV = m_shader->getShader("vertex").spirv;
    const auto& fragSPV = m_shader->getShader("fragment").spirv;

    if (vertSPV.empty() || fragSPV.empty()) {
        throw std::runtime_error("Shader SPIR-V is empty, compilation must have failed");
}

    gShaderProgram = glCreateProgram();

    const GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderBinary(1, &vertShader, GL_SHADER_BINARY_FORMAT_SPIR_V, vertSPV.data(), vertSPV.size() * sizeof(uint32_t));
    glSpecializeShader(vertShader, "main", 0, nullptr, nullptr);

    const GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderBinary(1, &fragShader, GL_SHADER_BINARY_FORMAT_SPIR_V, fragSPV.data(), fragSPV.size() * sizeof(uint32_t));
    glSpecializeShader(fragShader, "main", 0, nullptr, nullptr);

    glAttachShader(gShaderProgram, vertShader);
    glAttachShader(gShaderProgram, fragShader);
    glLinkProgram(gShaderProgram);

    GLint success = 0;
    glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &success);
    if (success == 0) {
        char log[512];
        glGetProgramInfoLog(gShaderProgram, 512, nullptr, log);
        throw std::runtime_error(log);
    }

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);
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
