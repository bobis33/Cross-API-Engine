#include "OPGL/OPGL.hpp"

#ifdef __linux__
#include "OPGL/Context/EGLContextLinux.hpp"
#elifdef _WIN32
#include "OPGL/Context/WGLContextWindows.hpp"
#elifdef __APPLE__
#include "OPGL/Context/NSGLContextMac.hpp"
#endif

#include <stdexcept>

void cae::OPGL::initialize(const NativeWindowHandle &nativeWindowHandle, const Color &clearColor)
{
#ifdef __linux__
    m_context = std::make_unique<EGLContextLinux>();
#elifdef _WIN32
    m_context = std::make_unique<WGLContextWindows>();
#elifdef __APPLE__
    m_context = std::make_unique<NSGLContextMac>();
#endif

    m_context->initialize(nativeWindowHandle);

    glEnable(GL_DEPTH_TEST);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
}

void cae::OPGL::draw(const WindowSize &windowSize, const ShaderID &shaderId)
{
    glViewport(0, 0, windowSize.width, windowSize.height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_programs.at(shaderId));
    glBindVertexArray(m_mesh.vao);
    glDrawArrays(GL_TRIANGLES, 0, m_mesh.vertexCount);
    glBindVertexArray(0);

    m_context->swapBuffers();
}

void cae::OPGL::createPipeline(const ShaderID &id, const ShaderIRModule &vertex, const ShaderIRModule &fragment)
{

    const GLuint program = glCreateProgram();

    GLuint vs = createGLShader(GL_VERTEX_SHADER, vertex);
    GLuint fs = createGLShader(GL_FRAGMENT_SHADER, fragment);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        throw std::runtime_error(log);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);

    m_programs[id] = program;
}

void cae::OPGL::createMesh(const std::vector<float> &vertices)
{
    Mesh mesh{};
    mesh.vertexCount = static_cast<GLsizei>(vertices.size() / 5);

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindVertexArray(mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_mesh = mesh;
}

GLuint cae::OPGL::createGLShader(const GLenum type, const ShaderIRModule &data)
{
    const GLuint shader = glCreateShader(type);

    glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, data.spirv.data(),
                   static_cast<GLsizei>(data.spirv.size() * sizeof(uint32_t)));

    glSpecializeShader(shader, data.entryPoint.c_str(), 0, nullptr, nullptr);

    return shader;
}
