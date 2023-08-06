#ifndef INC_2D_GAME_LEARN_GAME_H
#define INC_2D_GAME_LEARN_GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render.h"
#include "game_level.h"
#include "particle_gen.h"

// 表示当前游戏状态
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};
// 表示方向
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

const glm::vec2 PLAYER_SIZE(100, 20);
const GLfloat PLAYER_VELOCITY(500.0f);
const glm::vec2 BALL_VELOCITY(100.0f, -350.0f);
const GLfloat BALL_RADIUS = 12.5f;

typedef std::tuple<GLboolean, Direction, glm::vec2> Collision;

GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(glm::vec2 target);

class Game {
public:
    // 游戏状态
    GameState              m_State;
    GLboolean              m_Keys[1024];
    GLuint                 m_Width, m_Height;
    std::vector<GameLevel> m_Levels;
    GLuint                 m_Level;
    GameObject             *m_Player;
    BallObject             *m_Ball;
    ParticleGenerator      *m_PG;
    Renderer               *m_Renderer;

    // 构造/析构
    Game(GLuint width, GLuint height);

    ~Game();

    // 初始化游戏状态(夹子啊所有的着色器/纹理/关卡)
    void Init();

    // 重置游戏状态
    void ResetLevel();
    void ResetPlayer();

    // 游戏循环
    void ProcessInput(GLfloat dt);

    void Update(GLfloat dt);

    void Render();
    // 碰撞处理
    void DoCollisions();
};

#endif //INC_2D_GAME_LEARN_GAME_H
