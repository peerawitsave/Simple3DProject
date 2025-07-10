#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoord;

    Vertex(glm::vec3 pos, glm::vec3 color = glm::vec3(1.0f), glm::vec2 texCoord = glm::vec2(0.0f))
        : Position(pos), Color(color), TexCoord(texCoord) {
    }
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void Draw() const;

    // Factory method: create triangle mesh
    static Mesh* CreateTriangle();
    static Mesh* CreateQuad();

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};
