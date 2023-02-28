#pragma once

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "global.hpp"

class Shader {
    using fspath = std::filesystem::path;

  public:
    Shader(const fspath& vertexShaderPath, const fspath& fragmentShaderPath);
    Shader() = default;
    ~Shader();

    void setup(const fspath& vertexShaderPath, const fspath& fragmentShaderPath);
    void activate();

    void setUniform(const std::string& name, int x, int y) const {
        glUniform2i(glGetUniformLocation(m_programID, "offset"), x, y);
    }

    enum class ShaderType { Vertex, Fragment, Program };

    void verifyShaderCompile(unsigned int shader, ShaderType type);

    unsigned int m_programID;
};
