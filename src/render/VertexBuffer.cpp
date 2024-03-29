#include "VertexBuffer.h"

namespace rigel {
    VertexBuffer::VertexBuffer(const void *data, unsigned int size) {
        glGenBuffers(1, &m_bufferId);
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_bufferId);
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
