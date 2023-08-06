#ifndef INC_2D_GAME_LEARN_RENDER_H
#define INC_2D_GAME_LEARN_RENDER_H

#include "shader.h"
#include "texture.h"

//class Renderer {
//public:
//    virtual void Draw(Texture2D &texture,
//                      glm::vec2 position, glm::vec2 size,
//                      GLfloat rotate, glm::vec3 color) = 0;
//protected:
//    Shader m_shader;
//    GLuint m_quadVAO;
//};

class Renderer{
public:
    Renderer(Shader &shader);
    ~Renderer();

    void Draw(Texture2D *texture,
              glm::vec2 position, glm::vec2 size = glm::vec2(10, 10),
              GLfloat rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
private:
    Shader m_shader;
    GLuint m_quadVAO;
    void initRenderData();
};

#endif //INC_2D_GAME_LEARN_RENDER_H
