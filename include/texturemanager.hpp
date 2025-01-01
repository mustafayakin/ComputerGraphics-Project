#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <string>
#include <map>
#include <glad/glad.h>

namespace graf {

    class TextureManager {
    public:
        static bool addTextureFromFile(const std::string& name);
        static void activateTexture(const std::string& name, int unit=0);

    private:
        static std::map<std::string, GLuint> m_textures;
        static GLuint loadTexture(const std::string& path);
    };

}

#endif // TEXTUREMANAGER_HPP
