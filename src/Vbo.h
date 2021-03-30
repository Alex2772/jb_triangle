#pragma once


#include <GL/gl.h>
#include <cstddef>
#include "Vec.h"
#include "MyGLEW.h"
#include <vector>

/**
 * Vertex Buffer Object
 */
class Vbo {
private:
    GLuint mHandle;
    GLuint mAttribIndex;

    void setRawData(const char* data, size_t size);

public:

    Vbo(GLuint attribIndex);
    Vbo(const Vbo&) = delete;
    ~Vbo();

    void setData(const std::vector<vec2>& data);

    void bind();

    class EnableAttribArray {
    private:
        Vbo& mVbo;

    public:
        EnableAttribArray(Vbo& vbo);
        ~EnableAttribArray();


    };
};

