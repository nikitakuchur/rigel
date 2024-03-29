#include "TextureRectangle.h"

#include <glad/glad.h>

#include "stb_image.h"

namespace rigel {
    TextureRectangle::TextureRectangle(const std::string &path)
            : m_textureId(0),
              m_path(path),
              m_localBuffer(nullptr),
              m_width(0),
              m_height(0),
              m_bpp(0) {
        stbi_set_flip_vertically_on_load(1);
        m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_RECTANGLE, m_textureId);

        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
        glBindTexture(GL_TEXTURE_RECTANGLE, 0);

        if (m_localBuffer) {
            stbi_image_free(m_localBuffer);
        }
    }

    TextureRectangle::~TextureRectangle() {
        glDeleteTextures(1, &m_textureId);
    }

    void TextureRectangle::bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_RECTANGLE, m_textureId);
    }

    void TextureRectangle::unbind() const {
        glBindTexture(GL_TEXTURE_RECTANGLE, 0);
    }
}