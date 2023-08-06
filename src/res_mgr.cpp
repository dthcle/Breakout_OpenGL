#include <fstream>
#include <sstream>
#include <stb_image.h>
#include "res_mgr.h"

std::map<std::string, Shader>    ResMgr::m_Shaders;
std::map<std::string, Texture2D> ResMgr::m_Textures;

Shader& ResMgr::LoadShader(const GLchar *vShaderFilePath, const GLchar *fShaderFilePath, const GLchar *gShaderFilePath,
                          std::string name) {
    m_Shaders[name] = loadShaderFromFile(vShaderFilePath, fShaderFilePath, gShaderFilePath);
    return m_Shaders[name];
}

Shader& ResMgr::GetShader(std::string name) {
    return m_Shaders[name];
}

Texture2D& ResMgr::LoadTexture(const GLchar *file, GLboolean alpha, std::string name) {
    m_Textures[name] = loadTextureFromFile(file, alpha);
    return m_Textures[name];
}

Texture2D& ResMgr::GetTexture(std::string name) {
    return m_Textures[name];
}

void ResMgr::Clear() {
    for(auto iter: m_Shaders)
        glDeleteProgram(iter.second.m_ID);
    for(auto iter: m_Textures)
        glDeleteTextures(1, &iter.second.m_ID);
}

Shader ResMgr::loadShaderFromFile(const GLchar *vShaderFilePath, const GLchar *fShaderFilePath, const GLchar *gShaderFilePath) {
    // 1. get source from file_path
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try{
        // open file
        std::ifstream vertexShaderFile(vShaderFilePath);
        std::ifstream fragmentShaderFile(fShaderFilePath);
        std::stringstream vShaderStream, fShaderStream;
        // get file buffer
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert buffer to string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // geometry
        if(gShaderFilePath != nullptr){
            std::ifstream geometryShaderFile(gShaderFilePath);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    } catch(std::exception& e){
        std::cerr << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    Shader shader;
    shader.Compile(vertexCode.c_str(),
                   fragmentCode.c_str(),
                   geometryCode.length()? geometryCode.c_str(): nullptr);
    return shader;
}

Texture2D ResMgr::loadTextureFromFile(const GLchar *file, GLboolean alpha) {
    Texture2D texture;
    if(alpha){
        texture.m_Internal_Format = GL_RGBA;
        texture.m_Image_Format = GL_RGBA;
    }
    // load image
    int width, height, channels;
    unsigned char *image = stbi_load(file, &width, &height, &channels, 0);
    if(image) {
        texture.Generate(width, height, image);
    } else {
        std::cerr << "Load texture error!" << std::endl;
    }
    stbi_image_free(image);
    return texture;
}
