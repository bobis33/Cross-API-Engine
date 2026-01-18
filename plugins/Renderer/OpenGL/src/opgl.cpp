#include "OPGL/OPGL.hpp"

#ifdef __linux__
#include "OPGL/Context/EGLContextLinux.hpp"
#elifdef _WIN32
#include "OPGL/Context/WGLContextWindows.hpp"
#elifdef __APPLE__
#include "OPGL/Context/NSGLContextMac.hpp"
#endif

#include <glm/ext/matrix_transform.hpp>

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
    const auto &gl = m_context->gl;

    gl.Enable(GL_DEPTH_TEST);
    gl.ClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

    gl.GenBuffers(1, &m_ubo);
    gl.BindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    gl.BufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    gl.BindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);
    gl.BindBuffer(GL_UNIFORM_BUFFER, 0);
}

void cae::OPGL::draw(const WindowSize &windowSize, const ShaderID &shaderId, const glm::mat4 mvp)
{
    const auto &gl = m_context->gl;
    gl.Viewport(0, 0, windowSize.width, windowSize.height);
    gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gl.UseProgram(m_programs.at(shaderId));
    gl.BindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    gl.BufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &mvp);

    // binding = 0 car dans le shader: layout(binding = 0)
    gl.BindVertexArray(m_mesh.vao);
    gl.DrawArrays(GL_TRIANGLES, 0, m_mesh.vertexCount);
    gl.BindVertexArray(0);

    m_context->swapBuffers();
}

void cae::OPGL::createPipeline(const ShaderID &id, const ShaderIRModule &vertex, const ShaderIRModule &fragment)
{
    const auto &gl = m_context->gl;
    const GLuint program = gl.CreateProgram();

    const GLuint vs = createGLShader(GL_VERTEX_SHADER, vertex, gl);
    const GLuint fs = createGLShader(GL_FRAGMENT_SHADER, fragment, gl);

    gl.AttachShader(program, vs);
    gl.AttachShader(program, fs);
    gl.LinkProgram(program);

    GLint success = 0;
    gl.GetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        char log[512];
        gl.GetProgramInfoLog(program, 512, nullptr, log);
        throw std::runtime_error(log);
    }

    gl.DeleteShader(vs);
    gl.DeleteShader(fs);

    m_programs[id] = program;
}

void cae::OPGL::createMesh(const std::vector<float> &vertices)
{
    const auto &gl = m_context->gl;
    Mesh mesh{};
    mesh.vertexCount = static_cast<GLsizei>(vertices.size() / 5);

    gl.GenVertexArrays(1, &mesh.vao);
    gl.GenBuffers(1, &mesh.vbo);

    gl.BindVertexArray(mesh.vao);
    gl.BindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    gl.BufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    gl.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(0));
    gl.EnableVertexAttribArray(0);

    gl.VertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    gl.EnableVertexAttribArray(1);

    gl.BindBuffer(GL_ARRAY_BUFFER, 0);
    gl.BindVertexArray(0);

    m_mesh = mesh;
}

GLuint cae::OPGL::createGLShader(const GLenum type, const ShaderIRModule &data, const GladGLContext &gl)
{
    const GLuint shader = gl.CreateShader(type);

    gl.ShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, data.spirv.data(),
                    static_cast<GLsizei>(data.spirv.size() * sizeof(uint32_t)));

    gl.SpecializeShader(shader, data.entryPoint.c_str(), 0, nullptr, nullptr);

    return shader;
}
