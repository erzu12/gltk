#pragma once

#include "vec_math.h"
#include "color.h"
#include <functional>
#include <glad/glad.h>

#include <stdexcept>
#include <string>
#include <memory>

namespace gltk {


class GlResource {
    unsigned int resource;
    std::function<void(unsigned int)> deleter;
public:
    GlResource(unsigned int resource, std::function<void(unsigned int)> deleter) : resource(resource), deleter(deleter) {}
    ~GlResource() {
        deleter(resource);
    }

    operator unsigned int() const {
        return resource;
    }
};

class shader_exception : public std::runtime_error {
public:
    shader_exception(std::string message) : std::runtime_error(message) {}
};

class Shader {
public:
    std::unique_ptr<GlResource> shaderProgram;
    Shader(std::string vertexPath, std::string fragmentPath);
    void use() const;
    void UniformInt(std::string name, int value) const;
    void UniformFloat(std::string name, float value) const;
    void UniformVec2(std::string name, Vec2 vec) const;
    void UniformVec3(std::string name, Vec3 vec) const;
    void UniformColor(std::string name, Color color) const;
    void UniformMat3(std::string name, Mat3 mat) const;
private:
    static std::unique_ptr<GlResource> CompileShader(std::string path, GLenum shaderType);
    static char* LoadShader(std::string path);
};
}  // namespace neith

