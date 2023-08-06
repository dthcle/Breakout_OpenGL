#include <iostream>

#include "shader.h"

Shader &Shader::Use() {
    glUseProgram(m_ID);
    return *this;
}

void Shader::Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource) {
    GLuint sVertex, sFragment, gShader;
    // vertex
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, nullptr);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");
    // fragment
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, nullptr);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");
    // geometry
    if (geometrySource) {
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, nullptr);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }
    // program
    m_ID      = glCreateProgram();
    glAttachShader(m_ID, sVertex);
    glAttachShader(m_ID, sFragment);
    if (geometrySource) glAttachShader(m_ID, gShader);
    glLinkProgram(m_ID);
    checkCompileErrors(m_ID, "PROGRAM");
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    if (geometrySource) glDeleteShader(gShader);
}

void Shader::SetFloat(const GLchar *name, GLfloat value, GLboolean useShader) {
    if (useShader)
        Use();
    glUniform1f(glGetUniformLocation(m_ID, name), value);
}

void Shader::SetInteger(const GLchar *name, GLint value, GLboolean useShader) {
    if (useShader)
        Use();
    glUniform1i(glGetUniformLocation(m_ID, name), value);
}

void Shader::SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader) {
    if (useShader)
        Use();
    glUniform2f(glGetUniformLocation(m_ID, name), x, y);
}

void Shader::SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader) {
    if (useShader)
        Use();
    glUniform2f(glGetUniformLocation(m_ID, name), value.x, value.y);
}

void Shader::SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader) {
    if (useShader)
        Use();
    glUniform3f(glGetUniformLocation(m_ID, name), x, y, z);
}

void Shader::SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader) {
    if (useShader)
        Use();
    glUniform3f(glGetUniformLocation(m_ID, name), value.x, value.y, value.z);
}

void Shader::SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader) {
    if (useShader)
        Use();
    glUniform4f(glGetUniformLocation(m_ID, name), x, y, z, w);
}

void Shader::SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader) {
    if (useShader)
        Use();
    glUniform4f(glGetUniformLocation(m_ID, name), value.x, value.y, value.z, value.w);
}

void Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader) {
    if (useShader)
        Use();
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(GLuint object, std::string type) {
    GLint  success;
    GLchar infoLog[1024];
    {
        if (type != "PROGRAM") {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(object, 1024, nullptr, infoLog);
                std::cerr << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n" <<
                          infoLog << "\n -- --------------------------------------------------- -- " <<
                          std::endl;
            }
        } else {
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(object, 1024, nullptr, infoLog);
                std::cerr << "| ERROR::PROGRAM: Link-time error: Type: " << type << "\n" <<
                          infoLog << "\n -- --------------------------------------------------- -- " <<
                          std::endl;
            }
        }
    }
}
