#include "game_object.h"

GameObject::GameObject() :
        m_Position(0, 0), m_Size(1, 1), m_Velocity(0.0f),
        m_Color(1.0f), m_Rotation(0.0f), m_IsSolid(false),
        m_Destroyed(false), m_texture() {}

GameObject::GameObject(vec2 pos, vec2 size, Texture2D texture, vec3 color, vec2 velocity) :
        m_Position(pos), m_Size(size), m_Velocity(velocity),
        m_Color(color), m_Rotation(0.0f), m_IsSolid(false),
        m_Destroyed(false), m_texture(texture) {}

void GameObject::Draw(Renderer &renderer) {
    renderer.Draw(&m_texture, m_Position, m_Size, m_Rotation, m_Color);
}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D texture) :
        GameObject(pos, glm::vec2(radius*2, radius*2), texture, glm::vec3(1.0f), velocity), m_Radius(radius), m_Stuck(true) {}

glm::vec2 BallObject::Move(GLfloat dt, GLuint window_width){
    // 如果没有被固定在挡板上
    if(!m_Stuck){
        // 移动球
        m_Position += m_Velocity * dt;
        // 检查是否在窗口边界以外 如果是则反转速度并恢复到正确的位置
        if(m_Position.x <= 0.0f){
            m_Velocity.x = -m_Velocity.x;
            m_Position.x = 0.0f;
        } else if(m_Position.x + m_Size.x >= window_width){
            m_Velocity.x = -m_Velocity.x;
            m_Position.x = window_width - m_Size.x;
        }
        if(m_Position.y <= 0.0f){
            m_Velocity.y = -m_Velocity.y;
            m_Position.y = 0.0f;
        }
    }
    return m_Position;
}
