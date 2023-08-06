#ifndef INC_2D_GAME_LEARN_GAME_OBJECT_H
#define INC_2D_GAME_LEARN_GAME_OBJECT_H

#include <glad/glad.h>
#include "common.h"
#include "texture.h"
#include "render.h"

using namespace glm;

class GameObject {
public:
    // state
    vec2 m_Position, m_Size, m_Velocity;
    vec3 m_Color;
    GLfloat   m_Rotation;
    GLboolean m_IsSolid;
    GLboolean m_Destroyed;
    // render
    Texture2D m_texture;

    GameObject();
    GameObject(vec2 pos, vec2 size, Texture2D texture, vec3 color = vec3(1.0f), vec2 velocity = vec2(0.0f));

    // draw
    void Draw(Renderer &renderer);
};

class BallObject: public GameObject{
public:
    // 球的状态
    GLfloat m_Radius;
    GLboolean m_Stuck;

    BallObject();
    BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D texture);

    glm::vec2 Move(GLfloat dt, GLuint window_width);
    void Reset(glm::vec2 position, glm::vec2 velocity);
};

#endif //INC_2D_GAME_LEARN_GAME_OBJECT_H
