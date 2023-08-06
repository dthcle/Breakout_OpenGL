#ifndef INC_2D_GAME_LEARN_TEXTURE_H
#define INC_2D_GAME_LEARN_TEXTURE_H

#include <glad/glad.h>
#include <iostream>

class Texture2D{
public:
    GLuint m_ID;
    // image dimensions(loaded image in pixels)
    GLuint m_Width, m_Height;
    GLuint m_Internal_Format;  // texture object format
    GLuint m_Image_Format;     // loaded image format
    GLuint m_Wrap_S; // wrapping mode on S axis
    GLuint m_Wrap_T; // wrapping mode on T axis
    GLuint m_Filter_Min; // filter mode if texture pixels < screen pixels
    GLuint m_Filter_Max; // filter mode if texture pixels > screen pixels

    Texture2D();
    // generate texture from image data
    void Generate(GLuint width, GLuint height, unsigned char* data);
    // bind the texture with the current active GL_TEXTURE_2D
    void Bind() const;
};

#endif //INC_2D_GAME_LEARN_TEXTURE_H
