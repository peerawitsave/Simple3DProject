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
    // texcoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoord));
    glEnableVertexAttribArray(2);

    // normal attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(3);

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

Mesh* Mesh::CreateCircle(float radius, int segments) {
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    const float PI = 3.1415926f;

    // Center vertex at (0, 0, 0)
    verts.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 1, 1));

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * PI * i / segments;
        float x = cos(angle) * radius;
        float y = sin(angle) * radius;

        // Color gradient based on angle
        glm::vec3 color = glm::vec3(
            0.5f + 0.5f * cos(angle),
            0.5f + 0.5f * sin(angle),
            0.5f + 0.5f * cos(angle + PI * 0.5f)
        );

        verts.emplace_back(glm::vec3(x, y, 0.0f), color);

        if (i > 0) {
            inds.push_back(0);       // center
            inds.push_back(i);       // current
            inds.push_back(i + 1);   // next
        }
    }

    return new Mesh(verts, inds);
}

Mesh* Mesh::CreatePyramid() {
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    glm::vec3 baseColor = glm::vec3(1.0f, 0.7f, 0.2f);
    glm::vec3 topColor = glm::vec3(0.8f, 0.2f, 1.0f);

    // Vertex positions
    glm::vec3 p0(-0.5f, 0.0f, -0.5f);
    glm::vec3 p1(0.5f, 0.0f, -0.5f);
    glm::vec3 p2(0.5f, 0.0f, 0.5f);
    glm::vec3 p3(-0.5f, 0.0f, 0.5f);
    glm::vec3 apex(0.0f, 0.8f, 0.0f);

    glm::vec3 nBase(0.0f, -1.0f, 0.0f);

    glm::vec3 n0 = glm::normalize(glm::cross(p1 - p0, apex - p0));
    glm::vec3 n1 = glm::normalize(glm::cross(p2 - p1, apex - p1));
    glm::vec3 n2 = glm::normalize(glm::cross(p3 - p2, apex - p2));
    glm::vec3 n3 = glm::normalize(glm::cross(p0 - p3, apex - p3));

    // Base
    verts.emplace_back(p0, baseColor, glm::vec2(0.0f), nBase); // 0
    verts.emplace_back(p1, baseColor, glm::vec2(0.0f), nBase); // 1
    verts.emplace_back(p2, baseColor, glm::vec2(0.0f), nBase); // 2
    verts.emplace_back(p3, baseColor, glm::vec2(0.0f), nBase); // 3

    inds.insert(inds.end(), {
        0, 1, 2,
        0, 2, 3
        });

    // Side 0
    verts.emplace_back(p0, topColor, glm::vec2(0.0f), n0); // 4
    verts.emplace_back(p1, topColor, glm::vec2(0.0f), n0); // 5
    verts.emplace_back(apex, topColor, glm::vec2(0.0f), n0); // 6
    inds.insert(inds.end(), { 4, 5, 6 });

    // Side 1
    verts.emplace_back(p1, topColor, glm::vec2(0.0f), n1); // 7
    verts.emplace_back(p2, topColor, glm::vec2(0.0f), n1); // 8
    verts.emplace_back(apex, topColor, glm::vec2(0.0f), n1); // 9
    inds.insert(inds.end(), { 7, 8, 9 });

    // Side 2
    verts.emplace_back(p2, topColor, glm::vec2(0.0f), n2); // 10
    verts.emplace_back(p3, topColor, glm::vec2(0.0f), n2); // 11
    verts.emplace_back(apex, topColor, glm::vec2(0.0f), n2); // 12
    inds.insert(inds.end(), { 10, 11, 12 });

    // Side 3
    verts.emplace_back(p3, topColor, glm::vec2(0.0f), n3); // 13
    verts.emplace_back(p0, topColor, glm::vec2(0.0f), n3); // 14
    verts.emplace_back(apex, topColor, glm::vec2(0.0f), n3); // 15
    inds.insert(inds.end(), { 13, 14, 15 });

    return new Mesh(verts, inds);
}

Mesh* Mesh::CreateBackdropPlane() {
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    glm::vec3 normal(0.0f, 1.0f, 0.0f); // Upward normal
    glm::vec3 color(1.0f); // White

    verts.emplace_back(glm::vec3(-5, 0, -5), color, {}, normal);
    verts.emplace_back(glm::vec3(5, 0, -5), color, {}, normal);
    verts.emplace_back(glm::vec3(5, 0, 5), color, {}, normal);
    verts.emplace_back(glm::vec3(-5, 0, 5), color, {}, normal);

    inds = { 0, 1, 2, 0, 2, 3 };
    return new Mesh(verts, inds);
}




