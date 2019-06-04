#pragma once

#include "glutils/VertexArray.h"
#include "glutils/IndexBuffer.h"
#include "Shader.h"

namespace rigel
{
    class Renderer
    {
    public:
        void clear() const;
        void clear(float r, float g, float b) const;
        void drawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void drawArrays(const VertexArray& va, unsigned int count, const Shader& shader) const;
    };
}