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
    glm::vec3 normal(0, 0, 1);
    std::vector<Vertex> verts = {
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1, 0, 0), glm::vec2(0, 0), normal),
        Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0, 1, 0), glm::vec2(1, 0), normal),
        Vertex(glm::vec3(0.0f,  0.5f, 0.0f), glm::vec3(0, 0, 1), glm::vec2(0.5f, 1), normal)
    };
    std::vector<unsigned int> inds = { 0, 1, 2 };
    return new Mesh(verts, inds);
}


Mesh* Mesh::CreateQuad() {
    glm::vec3 normal(0, 0, 1);
    std::vector<Vertex> verts = {
        Vertex(glm::vec3(0.5f,  0.3f, 0.0f), glm::vec3(1, 0, 0), glm::vec2(1, 1), normal),
        Vertex(glm::vec3(0.5f, -0.3f, 0.0f), glm::vec3(0, 1, 0), glm::vec2(1, 0), normal),
        Vertex(glm::vec3(-0.5f, -0.3f, 0.0f), glm::vec3(0, 0, 1), glm::vec2(0, 0), normal),
        Vertex(glm::vec3(-0.5f,  0.3f, 0.0f), glm::vec3(1, 1, 0), glm::vec2(0, 1), normal)
    };
    std::vector<unsigned int> inds = { 0, 1, 2, 0, 2, 3 };
    return new Mesh(verts, inds);
}


Mesh* Mesh::CreateCircle(float radius, int segments) {
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    const float PI = 3.1415926f;

    // Center vertex at (0, 0, 0)
    verts.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1, 1, 1), glm::vec2(0), glm::vec3(0, 0, 1));


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

        verts.emplace_back(glm::vec3(x, y, 0.0f), color, glm::vec2(x, y), glm::vec3(0, 0, 1));

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

    // Positions
    glm::vec3 p0(-0.5f, 0.0f, -0.5f);
    glm::vec3 p1(0.5f, 0.0f, -0.5f);
    glm::vec3 p2(0.5f, 0.0f, 0.5f);
    glm::vec3 p3(-0.5f, 0.0f, 0.5f);
    glm::vec3 apex(0.0f, 0.8f, 0.0f);

    // Colors for each face
    glm::vec3 baseColor = glm::vec3(0.6f, 0.4f, 0.1f);  // brownish base
    glm::vec3 color0 = glm::vec3(1.0f, 0.0f, 0.0f);     // red
    glm::vec3 color1 = glm::vec3(0.0f, 1.0f, 0.0f);     // green
    glm::vec3 color2 = glm::vec3(0.0f, 0.0f, 1.0f);     // blue
    glm::vec3 color3 = glm::vec3(1.0f, 1.0f, 0.0f);     // yellow

    // Normals
    glm::vec3 nBase = glm::vec3(0.0f, -1.0f, 0.0f);
    glm::vec3 n0 = glm::normalize(glm::cross(p1 - p0, apex - p0));
    glm::vec3 n1 = glm::normalize(glm::cross(p2 - p1, apex - p1));
    glm::vec3 n2 = glm::normalize(glm::cross(p3 - p2, apex - p2));
    glm::vec3 n3 = glm::normalize(glm::cross(p0 - p3, apex - p3));

    // Base (2 triangles)
    verts.emplace_back(p0, baseColor, glm::vec2(0.0f), nBase);
    verts.emplace_back(p1, baseColor, glm::vec2(0.0f), nBase);
    verts.emplace_back(p2, baseColor, glm::vec2(0.0f), nBase);
    verts.emplace_back(p3, baseColor, glm::vec2(0.0f), nBase);
    inds.insert(inds.end(), { 0, 1, 2, 0, 2, 3 });

    // Side 0 - Red
    verts.emplace_back(p0, color0, glm::vec2(0.0f), n0);
    verts.emplace_back(p1, color0, glm::vec2(0.0f), n0);
    verts.emplace_back(apex, color0, glm::vec2(0.0f), n0);
    inds.insert(inds.end(), { 4, 5, 6 });

    // Side 1 - Green
    verts.emplace_back(p1, color1, glm::vec2(0.0f), n1);
    verts.emplace_back(p2, color1, glm::vec2(0.0f), n1);
    verts.emplace_back(apex, color1, glm::vec2(0.0f), n1);
    inds.insert(inds.end(), { 7, 8, 9 });

    // Side 2 - Blue
    verts.emplace_back(p2, color2, glm::vec2(0.0f), n2);
    verts.emplace_back(p3, color2, glm::vec2(0.0f), n2);
    verts.emplace_back(apex, color2, glm::vec2(0.0f), n2);
    inds.insert(inds.end(), { 10, 11, 12 });

    // Side 3 - Yellow
    verts.emplace_back(p3, color3, glm::vec2(0.0f), n3);
    verts.emplace_back(p0, color3, glm::vec2(0.0f), n3);
    verts.emplace_back(apex, color3, glm::vec2(0.0f), n3);
    inds.insert(inds.end(), { 13, 14, 15 });

    return new Mesh(verts, inds);
}

Mesh* Mesh::CreateBackdropPlane() {
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;

    glm::vec3 white(1.0f);                    // White color
    glm::vec3 normal(0.0f, 1.0f, 0.0f);       // Facing up

    float size = 5.0f;

    verts.emplace_back(glm::vec3(-size, 0.0f, -size), white, glm::vec2(0, 0), normal);
    verts.emplace_back(glm::vec3(size, 0.0f, -size), white, glm::vec2(1, 0), normal);
    verts.emplace_back(glm::vec3(size, 0.0f, size), white, glm::vec2(1, 1), normal);
    verts.emplace_back(glm::vec3(-size, 0.0f, size), white, glm::vec2(0, 1), normal);

    inds = { 0, 1, 2, 0, 2, 3 };

    return new Mesh(verts, inds);
}





