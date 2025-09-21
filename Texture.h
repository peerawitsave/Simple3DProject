#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& path, bool flipVertical = true);
    void bind(GLenum unit = GL_TEXTURE0) const;
    GLuint id() const { return m_id; }

private:
    GLuint m_id = 0;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;
};

