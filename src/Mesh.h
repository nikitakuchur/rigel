#ifndef RIGEL_MESH_H
#define RIGEL_MESH_H

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "glutils/VertexBuffer.h"
#include "glutils/IndexBuffer.h"
#include "glutils/VertexArray.h"
#include "glutils/Shader.h"

namespace rigel {
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoords;
        glm::vec3 normal;
    };

    class Mesh {
    private:
        VertexArray m_va;
        VertexBuffer m_vb;
        IndexBuffer m_ib;
        VertexBufferLayout m_layout;
    public:
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

        ~Mesh();

        inline VertexArray getVertexArray() const { return m_va; }

        inline IndexBuffer getIndexBuffer() const { return m_ib; }
    };
}

#endif //RIGEL_MESH_H
