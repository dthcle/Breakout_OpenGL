#ifndef INC_2D_GAME_LEARN_RES_MGR_H
#define INC_2D_GAME_LEARN_RES_MGR_H

#include <map>
#include <string>

#include "shader.h"
#include "texture.h"

class ResMgr{
public:
    // storage
    static std::map<std::string, Shader>    m_Shaders;
    static std::map<std::string, Texture2D> m_Textures;
    // load shader source
    static Shader& LoadShader(const GLchar *vDATA, const GLchar *fDATA, const GLchar *gDATA, std::string name);
    // get a stored shader
    static Shader& GetShader(std::string name);
    // load texture from image
    static Texture2D& LoadTexture(const GLchar *DATA, GLint DATA_length, GLboolean alpha, std::string name);
    // get a stored texture
    static Texture2D& GetTexture(std::string name);
    // clear all stored content
    static void Clear();
private:
    // 单例模式
    ResMgr() {}
    static Shader loadShaderFromResource(const GLchar *vDATA, const GLchar *fDATA, const GLchar *gDATA = nullptr);
    static Texture2D loadTextureFromResource(const GLchar *DATA, GLint DATA_length, GLboolean alpha);
};

#endif //INC_2D_GAME_LEARN_RES_MGR_H
