#include <glm/gtc/matrix_transform.hpp>
#include "render.h"
#include "common.h"

Renderer::Renderer(Shader &shader){
    m_shader = shader;
    initRenderData();
}

Renderer::~Renderer(){
    glDeleteVertexArrays(1, &m_quadVAO);
}

void Renderer::Draw(Texture2D *texture,
                    glm::vec2 position, glm::vec2 size,
                    GLfloat rotate, glm::vec3 color){
    glm::mat4 model = IM4;

    model = glm::translate(model, glm::vec3(position, 0.0f));

    // 为了将O_POINT移动到矩形的中心点(乘size是因为先做了缩放)
    model = glm::translate(model, glm::vec3(0.5f*size.x, 0.5f*size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), Z_AXIS);
    model = glm::translate(model, glm::vec3(-0.5f*size.x, -0.5f*size.y, 0.0f));

    model = glm::scale(model, glm::vec3(size, 1.0f));

    m_shader.Use();
    m_shader.SetMatrix4("model", model);
    m_shader.SetVector3f("color", color);

    if(texture) {
        glActiveTexture(GL_TEXTURE0);
        texture->Bind();
    }

    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::initRenderData(){
    // 配置VBO/VAO
    GLuint VBO;
    GLfloat vertices[] = {
            // 位置     // 纹理
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };

    // 绑定VAO以保存VBO内容
    glGenVertexArrays(1, &m_quadVAO);
    glBindVertexArray(m_quadVAO);

    // 读取VBO信息
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(m_quadVAO);
}
