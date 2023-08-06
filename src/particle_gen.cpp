#include "particle_gen.h"

ParticleGenerator::ParticleGenerator(Shader &shader, Texture2D &texture, GLuint amount):
shader(shader), texture(texture), amount(amount){
    init();
}

void ParticleGenerator::Update(GLfloat dt, GameObject &object, GLuint newParticles, glm::vec2 offset) {
    // add new particles
    for(GLuint i = 0; i < newParticles; i++){
        int unusedParticle = firstUnusedParticle();
        RespawnParticle(particles[unusedParticle], object, offset);
    }
    // update all particles
    for(Particle &particle: particles){
        particle.m_Life -= dt;
        if(particle.m_Life > 0.0f){
            particle.m_Position -= particle.m_Velocity * dt;
            particle.m_Color.a -= dt * 2.5;
        }
    }
}

void ParticleGenerator::init() {
    // 构建VBO
    GLuint VBO;
    GLfloat particle_quad[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    for(int i = 0; i < amount; i++){
        particles.emplace_back();
    }
}

GLuint ParticleGenerator::firstUnusedParticle(){
    static GLuint lastUsedParticle = 0;
    // 从lastUsedParticle开始搜索 直到找到第一个生命周期结束的粒子
    for(int i = lastUsedParticle; i < nr_particles; i++){
        if(particles[i].m_Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // 否则从头进行搜索
    for(int i = 0; i < lastUsedParticle; i++){
        if(particles[i].m_Life <= 0.0f){
            lastUsedParticle = i;
            return i;
        }
    }
    // 否则表示没有粒子需要重绘
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::RespawnParticle(Particle &particle, GameObject &object, glm::vec2 offset){
    GLfloat random = ((rand() % 100) - 50) / 10.0f;
    GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
    particle.m_Position = object.m_Position + random + offset;
    particle.m_Color = glm::vec4(glm::vec3(rColor), 1.5f);
    particle.m_Life = 1.0f;
    particle.m_Velocity = object.m_Velocity * 0.1f;
}

void ParticleGenerator::Draw() {
    // use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    shader.Use();
    for(Particle &particle: particles){
        if(particle.m_Life > 0.0f){
            shader.SetVector2f("offset", particle.m_Position);
            shader.SetVector4f("color", particle.m_Color);
            texture.Bind();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }
    // don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
