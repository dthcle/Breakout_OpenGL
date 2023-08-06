#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "game.h"
#include "res_mgr.h"

void error_callback(int error, const char *msg);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

Game *Breakout;

int main() {
    glfwSetErrorCallback(error_callback);
    if (glfwInit() != GL_TRUE) {
        std::cerr << "Error initialising GLFW!" << std::endl;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __DEBUG__
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    const int screen_width = 800;
    const int screen_height = 600;
    GLFWwindow *window = glfwCreateWindow(screen_width, screen_height, "OpenGL - Dt", nullptr, nullptr);
    if(window == nullptr){
        std::cerr << "Open glfw window failed!" << std::endl;
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(window);

    // 初始化glad 很重要！
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 检测init错误 若有其他错误皆来自本程序
    glGetError();

    glViewport(0, 0, screen_width, screen_height);
    // 关闭z轴的深度影响
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    Breakout = new Game(screen_width, screen_height);
    Breakout->Init();

    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    Breakout->m_State = GAME_ACTIVE;

    while(!glfwWindowShouldClose(window)){
        GLfloat currFrame = glfwGetTime();
        deltaTime = currFrame - lastFrame;
        lastFrame = currFrame;

        // manage input
        Breakout->ProcessInput(deltaTime);
        // update game state
        Breakout->Update(deltaTime);

        // render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        Breakout->Render();

        glfwSwapBuffers(window);
        glfwPollEvents();  // 处理所有未处理的信号
    }


    delete Breakout;

    glfwTerminate();
    return 0;
}

void error_callback(int error, const char *msg) {
    std::string s;
    s = "[" + std::to_string(error) + "]" + msg + '\n';
    std::cerr << s << std::endl;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode){
    if((key == GLFW_KEY_ESCAPE) && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    } else if(key >= 0 && key < 1024){
        if(action == GLFW_PRESS){
            Breakout->m_Keys[key] = GL_TRUE;
        } else if(action == GLFW_RELEASE){
            Breakout->m_Keys[key] = GL_FALSE;
        }
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}


