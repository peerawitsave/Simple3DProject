#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& verts, const std::vector<unsigned int>& inds)
    : vertices(verts), indices(inds) {
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
    glEnableVertexAttribArray(1);
    // texcoord (optional)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::Draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh* Mesh::CreateTriangle() {
    std::vector<Vertex> verts = {
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1, 0, 0)),
        Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0, 1, 0)),
        Vertex(glm::vec3(0.0f,  0.5f, 0.0f), glm::vec3(0, 0, 1))
    };

    std::vector<unsigned int> inds = { 0, 1, 2 };

    return new Mesh(verts, inds);
}

Mesh* Mesh::CreateQuad() {
    std::vector<Vertex> verts = {
        Vertex(glm::vec3(0.5f,  0.3f, 0.0f), glm::vec3(1, 0, 0)), // top right
        Vertex(glm::vec3(0.5f, -0.3f, 0.0f), glm::vec3(0, 1, 0)), // bottom right
        Vertex(glm::vec3(-0.5f, -0.3f, 0.0f), glm::vec3(0, 0, 1)), // bottom left
        Vertex(glm::vec3(-0.5f,  0.3f, 0.0f), glm::vec3(1, 1, 0))  // top left
    };

    std::vector<unsigned int> inds = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    return new Mesh(verts, inds);
}
