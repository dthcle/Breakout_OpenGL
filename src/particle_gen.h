#ifndef INC_2D_GAME_LEARN_PARTICLE_GEN_H
#define INC_2D_GAME_LEARN_PARTICLE_GEN_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "game_object.h"

const GLuint nr_particles = 500;

struct Particle {
    glm::vec2 m_Position, m_Velocity;
    glm::vec4 m_Color;
    GLfloat m_Life;

    Particle() :
            m_Position(0.0f), m_Velocity(0.0f), m_Color(1.0f), m_Life(0.0f) {};
};

class ParticleGenerator{
public:
    ParticleGenerator(Shader &shader, Texture2D &texture, GLuint amount);
    void Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
    void Draw();
private:
    std::vector<Particle> particles;
    GLuint amount;
    Shader shader;
    Texture2D texture;
    GLuint VAO;
    void init();
    GLuint firstUnusedParticle();
    void RespawnParticle(Particle &particle, GameObject &object, glm::vec2 offset);
};


#endif //INC_2D_GAME_LEARN_PARTICLE_GEN_H
