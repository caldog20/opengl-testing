#include "shader.hpp"

#include "GL/glew.h"

Shader::Shader(const fspath& vertexShaderPath, const fspath& fragmentShaderPath) {
    setup(vertexShaderPath, fragmentShaderPath);
}
void Shader::verifyShaderCompile(unsigned int shader, ShaderType type) {
    int result;
    char log[1024];

    if (type == ShaderType::Program) {
        glGetProgramiv(shader, GL_LINK_STATUS, &result);
        if (result != GL_TRUE) {
            glGetProgramInfoLog(shader, 1024, NULL, log);
            panic("Error Linking Shader Program: {}\n", log);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
        std::string shaderType;
        if (result != GL_TRUE) {
            if (type == ShaderType::Vertex) {
                shaderType = "Vertex";
            } else if (type == ShaderType::Fragment) {
                shaderType = "Fragment";
            }

            panic("Error Compiling {} Shader: {}\n", shaderType, log);
        }
    }
}

void Shader::activate() { glUseProgram(m_programID); }
Shader::~Shader() { glDeleteProgram(m_programID); }
void Shader::setup(const fspath& vertexShaderPath, const fspath& fragmentShaderPath) {
    std::string vertexSource, fragmentSource;

    std::ifstream vertexFile, fragmentFile;
    vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vertexFile.open(vertexShaderPath);
        std::stringstream vss;
        vss << vertexFile.rdbuf();
        vertexFile.close();
        vertexSource = vss.str();
    } catch (std::ifstream::failure& error) {
        panic("Failed to load Vertex Shader: {} ", error.what());
    }

    try {
        fragmentFile.open(fragmentShaderPath);
        std::stringstream fss;
        fss << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentSource = fss.str();
    } catch (std::ifstream::failure& error) {
        panic("Failed to load Fragment Shader: {}", error.what());
    }

    const char* vertexShaderSourceFinal = vertexSource.c_str();
    const char* fragmentShaderSourceFinal = fragmentSource.c_str();

    unsigned int vertexShader, fragmentShader;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceFinal, NULL);
    glCompileShader(vertexShader);
    verifyShaderCompile(vertexShader, ShaderType::Vertex);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceFinal, NULL);
    glCompileShader(fragmentShader);
    verifyShaderCompile(fragmentShader, ShaderType::Fragment);

    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);
    verifyShaderCompile(m_programID, ShaderType::Program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

//    const auto vramSamplerLoc = GL::uniformLocation(m_programID, "vramTex");
//    glUniform1i(glGetUniformLocation(m_programID, "u_vramTex"), 0);
//    glUniform4i(glGetUniformLocation(m_programID, "u_texWindow"), 0, 0, 0, 0);

}
