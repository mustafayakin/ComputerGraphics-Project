#define STB_IMAGE_IMPLEMENTATION   // Bir kere .cpp içinde olmalı
#include <stb_image.h>           
#include "texturemanager.hpp"
#include <iostream>
#include <vector>
#include <glad/glad.h>            // glGenTextures vs.
#include <string>

namespace graf {

std::map<std::string, GLuint> TextureManager::m_textures;

bool TextureManager::addTextureFromFile(const std::string& name)
{
    // Daha önce aynı isimle yüklendiyse tekrar yükleme
    if (m_textures.find(name) != m_textures.end()) {
        return true;
    }

    // images/<name> 
    std::string path = "./images/" + name;
    GLuint id = loadTexture(path);
    if (!id) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        return false;
    }

    // Haritaya ekle
    m_textures[name] = id;
    return true;
}

void TextureManager::activateTexture(const std::string& name, int unit)
{
    // glActiveTexture
    glActiveTexture(GL_TEXTURE0 + unit);

    auto it = m_textures.find(name);
    if (it == m_textures.end()) {
        // Bulunamadıysa 0 ID
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        glBindTexture(GL_TEXTURE_2D, it->second);
    }
}

GLuint TextureManager::loadTexture(const std::string& path)
{
    // 1) Dosyayı stb_image ile aç
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "stbi_load failed for: " << path << std::endl;
        return 0;
    }

    // 2) glGenTextures
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // 3) Texture verisini GPU’ya yükle
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    // (İsterseniz mipmap)
    // glGenerateMipmap(GL_TEXTURE_2D);

    // 4) Filtre ve wrap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);     
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    // 5) Belleği serbest bırak
    stbi_image_free(data);

    return texID;
}

} // namespace graf
