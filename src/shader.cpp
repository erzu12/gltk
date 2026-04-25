#include "shader.h"

#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>

namespace gltk {

std::unique_ptr<GlResource> Shader::CompileShader(const std::string &shaderCode, GLuint shaderType) {

    auto shader = std::make_unique<GlResource>(glCreateShader(shaderType), glDeleteShader);

    const char *code = shaderCode.c_str();
    glShaderSource(*shader.get(), 1, &code, NULL);
    glCompileShader(*shader.get());

    int success;
    char infoLog[512];
    glGetShaderiv(*shader.get(), GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(*shader.get(), 512, NULL, infoLog);
        throw shader_exception(
            "shader: " + std::to_string(*shader.get()) + " failed to compile with error message: " + infoLog
        );
    }

    return shader;
}

Shader::Shader(const std::string &vertexShaderStr, const std::string &fragmentShaderStr) {
    auto fragmentShader = CompileShader(fragmentShaderStr, GL_FRAGMENT_SHADER);
    auto vertexShader = CompileShader(vertexShaderStr, GL_VERTEX_SHADER);

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

void Shader::UniformInt(std::string name, int value) const {
    glUniform1i(glGetUniformLocation(*shaderProgram.get(), name.c_str()), value);
}

void Shader::UniformFloat(std::string name, float value) const {
    glUniform1f(glGetUniformLocation(*shaderProgram.get(), name.c_str()), value);
}

void Shader::UniformVec2(std::string name, Vec2 vec) const {
    glUniform2f(glGetUniformLocation(*shaderProgram.get(), name.c_str()), vec.x, vec.y);
}

void Shader::UniformVec3(std::string name, Vec3 vec) const {
    glUniform3f(glGetUniformLocation(*shaderProgram.get(), name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::UniformColor(std::string name, Color color) const {
    glUniform4f(glGetUniformLocation(*shaderProgram.get(), name.c_str()), color.r, color.g, color.b, color.a);
}

void Shader::UniformMat3(std::string name, Mat3 mat) const {
    glUniformMatrix3fv(glGetUniformLocation(*shaderProgram.get(), name.c_str()), 1, GL_TRUE, mat.data());
}

void Shader::use() const { glUseProgram(*shaderProgram.get()); }

} // namespace gltk
