#include "shader.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <sstream>


namespace gltk {

std::unique_ptr<GlResource> Shader::CompileShader(std::string shaderPath, GLuint shaderType) {
    std::string computeCode;
    std::ifstream cShaderFile;
    // ensure ifstream objects can throw exceptions:
    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        cShaderFile.open(shaderPath);
        std::stringstream cShaderStream;
        cShaderStream << cShaderFile.rdbuf();
        cShaderFile.close();
        computeCode = cShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        throw shader_exception("failed to read shader file: " + shaderPath);
    }


    auto shader = std::make_unique<GlResource>(glCreateShader(shaderType), glDeleteShader);

    const char* shaderCode = computeCode.c_str();
    glShaderSource(*shader.get(), 1, &shaderCode, NULL);
    glCompileShader(*shader.get());

    int success;
    char infoLog[512];
    glGetShaderiv(*shader.get(), GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(*shader.get(), 512, NULL, infoLog);
        throw shader_exception("shader: " + std::to_string(*shader.get()) + " failed to compile with error message: " + infoLog);
    }

    return shader;
}

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
    auto fragmentShader = CompileShader(fragmentPath, GL_FRAGMENT_SHADER);
    auto vertexShader = CompileShader(vertexPath, GL_VERTEX_SHADER);

    // Shader Program
    shaderProgram = std::make_unique<GlResource>(glCreateProgram(), glDeleteProgram);

    glAttachShader(*shaderProgram.get(), *vertexShader.get());
    glAttachShader(*shaderProgram.get(), *fragmentShader.get());
    glLinkProgram(*shaderProgram.get());

    int success;
    char infoLog[512];
    glGetProgramiv(*shaderProgram.get(), GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*shaderProgram.get(), 512, NULL, infoLog);
        throw shader_exception("shader_program linking faild " + std::string(infoLog));
    }
}

void Shader::use() {
    glUseProgram(*shaderProgram.get());
}

}  // namespace neith

