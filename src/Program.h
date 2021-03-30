#pragma once


#include <GL/gl.h>
#include <string>
#include <vector>
#include "MyGLEW.h"

/**
 * Represents an OpenGL program (shader)
 */
class Program {
private:
    GLuint mProgram;
    GLuint mVertex = 0;
    GLuint mFragment = 0;


    static GLuint load(const std::string& source, gl::ShaderType type);

public:
    Program(const std::string& vertex, const std::string& fragment, const std::vector<std::string>& attribs);
    Program(const Program&);

    void use();

    ~Program();
};

