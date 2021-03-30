//
// Created by alex2772 on 3/30/21.
//

#include "Vbo.h"
#include "MyGLEW.h"

Vbo::Vbo(GLuint attribIndex):
    mAttribIndex(attribIndex)
{
    gl::genBuffers(1, &mHandle);
}

Vbo::~Vbo() {
    gl::deleteBuffers(1, &mHandle);
}

void Vbo::bind() {
    static Vbo* lastBound = nullptr;
    if (lastBound != this) {
        lastBound = this;
        gl::bindBuffer(gl::ARRAY_BUFFER, mHandle);
    }
}

void Vbo::setRawData(const char* data, size_t size) {
    bind();
    gl::bufferData(gl::ARRAY_BUFFER, size, data, gl::STATIC_DRAW);
}

void Vbo::setData(const std::vector<vec2>& data) {
    setRawData(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(vec2));
    gl::vertexAttribPointer(mAttribIndex, 2, gl::FLOAT, true, 0, nullptr);
}

Vbo::EnableAttribArray::EnableAttribArray(Vbo& vbo) : mVbo(vbo) {
    gl::enableVertexAttribArray(mVbo.mAttribIndex);
}

Vbo::EnableAttribArray::~EnableAttribArray() {
    gl::disableVertexAttribArray(mVbo.mAttribIndex);
}
