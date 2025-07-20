#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Color;
    glm::vec2 TexCoord;
    glm::vec3 Normal;

    Vertex(glm::vec3 pos, glm::vec3 col)
        : Position(pos), Color(col), TexCoord(glm::vec2(0.0f)), Normal(glm::vec3(0.0f, 0.0f, 1.0f)) {
    }

    Vertex(glm::vec3 pos, glm::vec3 col, glm::vec2 uv, glm::vec3 norm)
        : Position(pos), Color(col), TexCoord(uv), Normal(norm) {
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
    static Mesh* CreateCircle(float radius = 0.5f, int segments = 64);
    static Mesh* CreatePyramid();
    static Mesh* CreateBackdropPlane();

private:
    unsigned int VAO, VBO, EBO;
    void setupMesh();
};
