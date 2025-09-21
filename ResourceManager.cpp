#include "ResourceManager.h"

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string& path)
{
    auto it = m_textures.find(path);
    if (it != m_textures.end()) {
        if (auto existing = it->second.lock()) {
            return existing;
        }
    }

    auto tex = std::make_shared<Texture>();
    if (!tex->loadFromFile(path)) {
        return nullptr;
    }
    m_textures[path] = tex;
    return tex;
}

void ResourceManager::clear()
{
    m_textures.clear();
}

