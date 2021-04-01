#pragma once


#include <GL/gl.h>

/**
 * Custom implementation of openGL Extension Wrapper
 */

namespace gl {

    enum ShaderType: GLenum {
        FRAGMENT_SHADER = 0x8B30,
        VERTEX_SHADER = 0x8B31,
    };

    enum BufferTarget: GLenum {
        ARRAY_BUFFER = 0x8892,
        ELEMENT_ARRAY_BUFFER = 0x8893,
    };

    enum Usage: GLenum {
        STATIC_DRAW = 0x88E4,
        DYNAMIC_DRAW = 0x88E8,
        STREAM_DRAW = 0x88E0,
    };

    enum ShaderivPName: GLenum {
        COMPILE_STATUS = 0x8B81,
    };

    enum Type: GLenum {
        BYTE = 0x1400,
        UNSIGNED_BYTE = 0x1401,
        SHORT = 0x1402,
        UNSIGNED_SHORT= 0x1403,
        INT = 0x1404,
        UNSIGNED_INT = 0x1405,
        FLOAT = 0x1406,
        B2 = 0x1407,
        B3 = 0x1408,
        B4 = 0x1409,
        DOUBLE = 0x140A,
    };

    enum PrimitiveType: GLenum {
        POINTS = 0x0000,
        LINES = 0x0001,
        LINE_LOOP = 0x0002,
        LINE_STRIP = 0x0003,
        TRIANGLES = 0x0004,
        TRIANGLE_STRIP = 0x0005,
        TRIANGLE_FAN = 0x0006,
        QUADS = 0x0007,
        QUAD_STRIP = 0x0008,
        POLYGON = 0x0009,
    };

    static_assert(sizeof(bool) == sizeof(unsigned char), "sizeof(bool) != sizeof(unsigned char)");

    // vbo
    extern void (*genBuffers)(GLsizei n, GLuint* buffers);
    extern void (*bindBuffer)(BufferTarget target, GLuint buffer);
    extern void (*bufferData)(BufferTarget target, GLsizeiptr size, const void * data, Usage usage);
    extern void (*vertexAttribPointer)(GLuint index, GLint size, Type type, bool normalized, GLsizei stride, const void * pointer);
    extern void (*drawArrays)(gl::PrimitiveType mode, GLint first, GLsizei count);
    extern void (*deleteBuffers)(GLsizei n, GLuint* buffers);
    extern void (*enableVertexAttribArray)(GLuint index);
    extern void (*disableVertexAttribArray)(GLuint index);

    // program
    extern GLuint (*createProgram)();
    extern void (*useProgram)(GLuint program);
    extern void (*deleteProgram)(GLuint program);
    extern void (*linkProgram)(GLuint shader);
    extern void (*bindAttribLocation)(GLuint program, GLuint index, const GLchar *name);

    // shader
    extern GLuint (*createShader)(ShaderType type);
    extern void (*shaderSource)(GLuint shader, GLsizei count, const GLchar** source, const GLint* length);
    extern void (*attachShader)(GLuint program, GLuint shader);
    extern void (*detachShader)(GLuint program, GLuint shader);
    extern void (*compileShader)(GLuint shader);
    extern void (*getShaderiv)(GLuint shader, ShaderivPName pname, GLint* params);
    extern void (*getShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
    extern void (*deleteShader)(GLuint shader);

    // shader uniform
    extern GLint (*getUniformLocation)(GLuint program, const GLchar* name);
    extern void (*uniform1f)(GLint location, GLfloat v0);
    extern void (*uniform2f)(GLint location, GLfloat v0, GLfloat v1);


    void init();
};

