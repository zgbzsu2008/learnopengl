#ifndef LEARNOPENGL_SHADER_HPP
#define LEARNOPENGL_SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <cassert>
#include <memory>
#include <iostream>

namespace learnopengl {
class Shader
{
public:
    Shader() { program_ = glCreateProgram(); }
    ~Shader() { glDeleteProgram(program_); }

    Shader& activate();
    Shader& attach(const std::string& filename, const std::string& path = "./");
    GLuint create(const std::string& filename);
    GLuint get() { return program_; }
    Shader& link();

    void bind(unsigned int location, int value);
    void bind(unsigned int location, float value);
    void bind(unsigned int location, const glm::vec4& value);
    void bind(unsigned int location, const glm::mat4& matrix);
    template<typename T>
    Shader& bind(const std::string& name, T&& value)
    {
        int location = glGetUniformLocation(program_, name.c_str());
        if (location == -1) {
            std::cout << "Missing Uniform: " << name << std::endl;
        } else {
            bind(location, std::move(value));
        }
        return *this;
    }

private:
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    GLuint program_;
    GLint status_;
    GLint length_;
};

Shader& Shader::activate()
{
    glUseProgram(program_);
    return *this;
}

Shader& Shader::attach(const std::string& filename, const std::string& path)
{
    std::ifstream fd(path + filename);
    std::string src{ std::istreambuf_iterator<char>(fd), std::istreambuf_iterator<char>() };
    const char* source = src.c_str();

    GLuint shader = create(filename);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status_);
    if (!status_) {
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length_);
        std::unique_ptr<char[]> buffer(new char[length_]);
        glGetProgramInfoLog(program_, length_, nullptr, buffer.get());
        std::cout << filename << std::endl << buffer.get() << std::endl;
    }

    glAttachShader(program_, shader);
    glDeleteShader(shader);
    return *this;
}

GLuint Shader::create(const std::string& filename)
{
    auto index = filename.rfind(".");
    auto ext = filename.substr(index + 1);
    if (ext == "comp") {
#ifdef GL_COMPUTE_SHADER
        return glCreateShader(GL_COMPUTE_SHADER);
#endif
    } else if (ext == "frag") {
        return glCreateShader(GL_FRAGMENT_SHADER);
    } else if (ext == "geom") {
        return glCreateShader(GL_GEOMETRY_SHADER);
    } else if (ext == "vert") {
        return glCreateShader(GL_VERTEX_SHADER);
    }

    return false;
}

Shader& Shader::link()
{
    glLinkProgram(program_);
    glGetProgramiv(program_, GL_LINK_STATUS, &status_);
    if (!status_) {
        glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length_);
        std::unique_ptr<char[]> buffer(new char[length_]);
        glGetProgramInfoLog(program_, length_, nullptr, buffer.get());
        std::cout << buffer.get() << std::endl;
    }
    assert(status_);
    return *this;
}

void Shader::bind(unsigned int location, int value)
{
    glUniform1i(location, value);
}

void Shader::bind(unsigned int location, float value)
{
    glUniform1f(location, value);
}

void Shader::bind(unsigned int location, const glm::vec4& value)
{
    glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::bind(unsigned int location, const glm::mat4& matrix)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

} // learnopengl

#endif // !LEARNOPENGL_SHADER_HPP
