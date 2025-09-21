#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "Texture.h"

class ResourceManager
{
public:
    std::shared_ptr<Texture> getTexture(const std::string& path);
    void clear();

private:
    std::unordered_map<std::string, std::weak_ptr<Texture>> m_textures;
};

