//
// Created by alex2772 on 3/30/21.
//

#include <GL/glx.h>
#include <iostream>
#include "MyGLEW.h"


// vbo
void (*gl::genBuffers)(GLsizei n, GLuint* buffers) = nullptr;
void (*gl::bindBuffer)(gl::BufferTarget target, GLuint buffer) = nullptr;
void (*gl::bufferData)(gl::BufferTarget target, GLsizeiptr size, const void * data, gl::Usage usage) = nullptr;
void (*gl::vertexAttribPointer)(GLuint index, GLint size, gl::Type type, bool normalized, GLsizei stride, const void * pointer) = nullptr;
void (*gl::drawArrays)(gl::PrimitiveType mode, GLint first, GLsizei count) = nullptr;
void (*gl::deleteBuffers)(GLsizei n, GLuint* buffers) = nullptr;
void (*gl::enableVertexAttribArray)(GLuint index) = nullptr;
void (*gl::disableVertexAttribArray)(GLuint index) = nullptr;

// program
GLuint (*gl::createProgram)() = nullptr;
void (*gl::useProgram)(GLuint program) = nullptr;
void (*gl::deleteProgram)(GLuint program) = nullptr;
void (*gl::linkProgram)(GLuint shader) = nullptr;
void (*gl::bindAttribLocation)(GLuint program, GLuint index, const GLchar *name) = nullptr;

// shader
GLuint (*gl::createShader)(gl::ShaderType type) = nullptr;
void (*gl::shaderSource)(GLuint shader, GLsizei count, const GLchar** source, const GLint* length) = nullptr;
void (*gl::attachShader)(GLuint program, GLuint shader) = nullptr;
void (*gl::detachShader)(GLuint program, GLuint shader) = nullptr;
void (*gl::compileShader)(GLuint shader) = nullptr;
void (*gl::getShaderiv)(GLuint shader, gl::ShaderivPName pname, GLint *params) = nullptr;
void (*gl::getShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) = nullptr;
void (*gl::deleteShader)(GLuint shader) = nullptr;

// shader uniform
GLint (*gl::getUniformLocation)(GLuint program, const GLchar* name) = nullptr;
void (*gl::uniform1f)(GLint location, GLfloat v0) = nullptr;
void (*gl::uniform2f)(GLint location, GLfloat v0, GLfloat v1) = nullptr;

template<typename T>
void getProcAddress(T& dst, const char* procName) {
    dst = reinterpret_cast<T>(glXGetProcAddress(reinterpret_cast<const GLubyte*>(procName)));

    if (dst == nullptr) {
        std::cout << "warning: " << procName << " is not available" << std::endl;
    }
}

void gl::init() {
    // vbo
    getProcAddress(genBuffers, "glGenBuffers");
    getProcAddress(bindBuffer, "glBindBuffer");
    getProcAddress(bufferData, "glBufferData");
    getProcAddress(vertexAttribPointer, "glVertexAttribPointer");
    getProcAddress(drawArrays, "glDrawArrays");
    getProcAddress(deleteBuffers, "glDeleteBuffers");
    getProcAddress(enableVertexAttribArray, "glEnableVertexAttribArray");
    getProcAddress(disableVertexAttribArray, "glDisableVertexAttribArray");

    // program
    getProcAddress(createProgram, "glCreateProgram");
    getProcAddress(useProgram, "glUseProgram");
    getProcAddress(deleteProgram, "glDeleteProgram");
    getProcAddress(linkProgram, "glLinkProgram");
    getProcAddress(bindAttribLocation, "glBindAttribLocation");

    // shader
    getProcAddress(createShader, "glCreateShader");
    getProcAddress(shaderSource, "glShaderSource");
    getProcAddress(attachShader, "glAttachShader");
    getProcAddress(detachShader, "glDetachShader");
    getProcAddress(compileShader, "glCompileShader");
    getProcAddress(getShaderiv, "glGetShaderiv");
    getProcAddress(getShaderInfoLog, "glGetShaderInfoLog");
    getProcAddress(deleteShader, "glDeleteShader");

    // shader uniform
    getProcAddress(getUniformLocation, "glGetUniformLocation");
    getProcAddress(uniform1f, "glUniform1f");
    getProcAddress(uniform2f, "glUniform2f");
}
