#include "Shader.h"

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace rigel {
    Shader::Shader(const std::string &path)
            : m_path(path) {
        ShaderProgramSource programSource = parseShader(path);
        m_shaderId = createShader(programSource.vertexSource, programSource.fragmentSource);
    }

    Shader::~Shader() {
        glDeleteProgram(m_shaderId);
    }

    void Shader::bind() const {
        glUseProgram(m_shaderId);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    void Shader::setUniform(const std::string &name, int value) {
        bind();
        glUniform1i(getUniformLocation(name), value);
    }

    void Shader::setUniform(const std::string &name, float value) {
        bind();
        glUniform1f(getUniformLocation(name), value);
    }

    void Shader::setUniform(const std::string &name, const glm::vec3 &vec) {
        bind();
        glUniform3f(getUniformLocation(name), vec.x, vec.y, vec.z);
    }

    void Shader::setUniform(const std::string &name, const glm::vec4 &vec) {
        bind();
        glUniform4f(getUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
    }

    void Shader::setUniform(const std::string &name, const glm::mat4 &mat) {
        bind();
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    void Shader::setUniform(const std::string &name, const Texture &texture) {
        bind();
        texture.bind();
        setUniform(name, 0);
    }

    void Shader::setUniform(const std::string &name, const TextureRectangle &textureRectangle) {
        bind();
        textureRectangle.bind();
        setUniform(name, 0);
    }

    ShaderProgramSource Shader::parseShader(const std::string &path) {
        std::ifstream stream(path);

        enum class ShaderType {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;

        while (getline(stream, line)) {
            if (line.find("#shader") != std::string::npos) {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            } else {
                if (type != ShaderType::NONE)
                    ss[(int) type] << line << '\n';
            }
        }

        return {ss[0].str(), ss[1].str()};
    }

    unsigned int Shader::compileShader(unsigned int type, const std::string &source) {
        unsigned int shader = glCreateShader(type);
        const char *src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        // Error handling
        int success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE) {
            char message[512];
            glGetShaderInfoLog(shader, 512, nullptr, message);

            std::cout << "Failed to compile " <<
                      (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
            std::cout << message << std::endl;

            glDeleteShader(shader);

            return 0;
        }

        return shader;
    }

    unsigned int Shader::createShader(const std::string &vertexSource, const std::string &fragmentSource) {
        unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
        unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

        if (!vertexShader || !fragmentShader)
            return 0;

        unsigned int program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return program;
    }

    int Shader::getUniformLocation(const std::string &name) {
        if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
            return m_uniformLocationCache[name];

        int location = glGetUniformLocation(m_shaderId, name.c_str());
        if (location == -1)
            std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
        else
            m_uniformLocationCache[name] = location;
        return location;
    }
}