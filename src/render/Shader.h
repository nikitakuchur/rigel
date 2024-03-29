#ifndef RIGEL_SHADER_H
#define RIGEL_SHADER_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>
#include "TextureRectangle.h"
#include "Texture.h"

namespace rigel
{
    struct ShaderProgramSource
    {
        std::string vertexSource;
        std::string fragmentSource;
    };

    class Shader
    {
    private:
        unsigned int m_shaderId;
        std::string m_path; // File path
        std::unordered_map<std::string, int> m_uniformLocationCache;
    public:
        Shader(const std::string& path);
        ~Shader();

        void bind() const;
        void unbind() const;

        void setUniform(const std::string& name, int value);
        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, const glm::vec3& vec);
        void setUniform(const std::string& name, const glm::vec4& vec);
        void setUniform(const std::string& name, const glm::mat4& mat);
        void setUniform(const std::string& name, const Texture& texture);
        void setUniform(const std::string& name, const TextureRectangle& textureRectangle);

    private:
        static ShaderProgramSource parseShader(const std::string& path);
        static unsigned int compileShader(unsigned int type, const std::string& source);
        static unsigned int createShader(const std::string& vertexSource, const std::string& fragmentSource);
        int getUniformLocation(const std::string& name);
    };
}

#endif //RIGEL_SHADER_H
