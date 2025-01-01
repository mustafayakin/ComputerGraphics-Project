#ifndef SHADERMANAGER_HPP
#define SHADERMANAGER_HPP

#include <string>
#include <map>
#include "shaderprogram.hpp"

namespace graf {

    class ShaderManager {
    public:
        // Dosya okuyan fonksiyon
        static bool addShaderFromFile(const std::string& name);

        // Doğrudan kaynak kodu ile oluşturma
        static bool createFromSource(const std::string& name,
                                     const std::string& vsSrc,
                                     const std::string& fsSrc);

        static ShaderProgram* getProgram(const std::string& name);

    private:
        static std::map<std::string, ShaderProgram*> m_programs;
        static std::string readFile(const std::string& path);
    };

}

#endif // SHADERMANAGER_HPP
