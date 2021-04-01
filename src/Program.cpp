//
// Created by alex2772 on 3/30/21.
//

#include <stdexcept>
#include <iostream>
#include "Program.h"
#include "MyGLEW.h"



Program::Program(const std::string& vertex, const std::string& fragment, const std::vector<std::string>& attribs) {
    mProgram = gl::createProgram();

    auto const prefix = "#version 120\n";

    mVertex = load(prefix + vertex, gl::VERTEX_SHADER);
    mFragment = load(prefix + fragment, gl::FRAGMENT_SHADER);

    unsigned index = 0;
    for (auto& s : attribs) {
        gl::bindAttribLocation(mProgram, index++, s.c_str());
    }

    gl::attachShader(mProgram, mVertex);
    gl::attachShader(mProgram, mFragment);
    gl::linkProgram(mProgram);

}

Program::~Program() {
    gl::detachShader(mProgram, mVertex);
    gl::detachShader(mProgram, mFragment);
    gl::deleteShader(mVertex);
    gl::deleteShader(mFragment);
    gl::deleteProgram(mProgram);
}

GLuint Program::load(const std::string& source, gl::ShaderType type) {
    uint32_t shader = gl::createShader(type);
    const char* c = source.c_str();
    gl::shaderSource(shader, 1, &c, nullptr);
    gl::compileShader(shader);
    int st;
    gl::getShaderiv(shader, gl::COMPILE_STATUS, &st);

    {
        char buf[8192];
        GLsizei len;
        gl::getShaderInfoLog(shader, sizeof(buf), &len, buf);
        if (len) {
            std::cout << "Compile error: " << buf << std::endl;
        }
    }
    if (!st) {
        throw std::runtime_error("failed to compile shader");
    }

    return shader;

}

void Program::use() {
    static Program* lastUsedProgram = nullptr;
    if (lastUsedProgram != this) {
        lastUsedProgram = this;
        gl::useProgram(mProgram);
    }
}

void Program::set(const char* uniformName, const vec2& value) {
    use();
    gl::uniform2f(gl::getUniformLocation(mProgram, uniformName), value.x, value.y);
}
