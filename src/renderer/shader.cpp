#include "renderer/shader.hpp"

// Before glad: both headers define APIENTRY, and glad only guards its own
// definition with #ifndef, so windows.h has to get there first or MSVC warns
// about a macro redefinition.
#ifdef _WIN32
#include <windows.h>
#endif

#include <glad/glad.h>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace {
std::string info_log(
    unsigned int name,
    void (*get_iv)(GLuint, GLenum, GLint*),
    void (*get_log)(GLuint, GLsizei, GLsizei*, GLchar*)
) {
    int length = 0;
    get_iv(name, GL_INFO_LOG_LENGTH, &length);
    if (length <= 0) {
        // Some drivers report a compile failure with an empty log. Say so
        // rather than throwing a blank error.
        return "(driver returned no info log)";
    }
    std::vector<char> log(static_cast<std::size_t>(length));
    get_log(name, length, nullptr, log.data());
    return std::string(log.data());
}

void report(const std::string& msg) {
    std::cerr << msg << std::endl;
#ifdef _WIN32
    OutputDebugStringA(msg.c_str());
    OutputDebugStringA("\n");
#endif
}

} // namespace

unsigned int
compile_shader(unsigned int type, const char* source, const char* label) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        std::string log = info_log(shader, glGetShaderiv, glGetShaderInfoLog);
        glDeleteShader(shader);
        throw std::runtime_error(
            std::string("Shader compilation failed (") + label + "):\n" + log
        );
    }
    return shader;
}

unsigned int
link_program(unsigned int vertex_shader, unsigned int fragment_shader) {
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        std::string log =
            info_log(program, glGetProgramiv, glGetProgramInfoLog);
        glDeleteProgram(program);
        throw std::runtime_error(
            std::string("Shader program linking failed:\n") + log
        );
    }
    return program;
}

void report_fatal(const std::string& msg) {
    report(msg);
#ifdef _WIN32
    // Last resort: the app is a windowed process, so if it was started by
    // double-clicking the exe there is no stderr anywhere to read. A box makes
    // the failure impossible to miss.
    MessageBoxA(
        nullptr, msg.c_str(), "modul - startup failed", MB_OK | MB_ICONERROR
    );
#endif
}
