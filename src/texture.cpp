#include "texture.h"

Texture2D::Texture2D()
        : m_Width(0), m_Height(0),
          m_Internal_Format(GL_RGB), m_Image_Format(GL_RGB),
          m_Wrap_S(GL_REPEAT), m_Wrap_T(GL_REPEAT),
          m_Filter_Min(GL_LINEAR), m_Filter_Max(GL_LINEAR) {
    glGenTextures(1, &m_ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char *data) {
    m_Width = width;
    m_Height = height;
    // create texture
    glBindTexture(GL_TEXTURE_2D, m_ID);
    glTexImage2D(GL_TEXTURE_2D, 0, m_Internal_Format, width, height, 0, m_Image_Format, GL_UNSIGNED_BYTE, data);
    // set wrap/filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Filter_Max);
    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Bind() const {
    glBindTexture(GL_TEXTURE_2D, m_ID);
}
