#include "shadermanager.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

namespace graf {

std::map<std::string, ShaderProgram*> ShaderManager::m_programs;

std::string ShaderManager::readFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open shader file: " << path << std::endl;
        return "";
    }
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

bool ShaderManager::addShaderFromFile(const std::string& name)
{
    // Projenizde: ./shaders/<name>/settings.json formatında
    std::string jsonPath = "./shaders/" + name + "/settings.json";
    std::ifstream f(jsonPath);
    if (!f.is_open()) {
        std::cerr << "Shader settings not found: " << jsonPath << std::endl;
        return false;
    }

    nlohmann::json data = nlohmann::json::parse(f);
    f.close();

    // Okunan JSON, VertexShader, FragmentShader, Uniforms vb. içerir
    std::string vsFile = data["VertexShader"].get<std::string>();
    std::string fsFile = data["FragmentShader"].get<std::string>();

    // Dosya içeriğini oku
    std::string vsSource = readFile("./shaders/" + name + "/" + vsFile);
    std::string fsSource = readFile("./shaders/" + name + "/" + fsFile);

    if (vsSource.empty() || fsSource.empty()) {
        return false;
    }

    return createFromSource(name, vsSource, fsSource);
}

bool ShaderManager::createFromSource(const std::string& name,
                                     const std::string& vsSrc,
                                     const std::string& fsSrc)
{
    ShaderProgram* prog = new ShaderProgram();
    if (!prog->create(vsSrc, fsSrc)) {
        delete prog;
        return false;
    }
    m_programs[name] = prog;
    return true;
}

ShaderProgram* ShaderManager::getProgram(const std::string& name)
{
    auto it = m_programs.find(name);
    if (it != m_programs.end()) {
        return it->second;
    }
    return nullptr;
}

}
