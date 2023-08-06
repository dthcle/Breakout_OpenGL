#include "game.h"
#include "res_mgr.h"
#include "render.h"


Game::Game(GLuint width, GLuint height):
m_State(GAME_ACTIVE), m_Keys(), m_Width(width), m_Height(height){

}

Game::~Game(){
    delete m_Renderer;
    delete m_Player;
}

void Game::Init(){
    // 加载着色器
    ResMgr::LoadShader("res/shaders/shader.vert", "res/shaders/shader.frag", nullptr, "shader");
    ResMgr::LoadShader("res/shaders/particle.vert", "res/shaders/particle.frag", nullptr, "particle");

    // 配置着色器
    glm::mat4 projection = glm::ortho(
            0.0f, static_cast<GLfloat>(m_Width),
            static_cast<GLfloat>(m_Height), 0.0f,
            -1.0f, 1.0f);
    ResMgr::GetShader("shader").Use().SetInteger("image", 0);
    ResMgr::GetShader("shader").SetMatrix4("projection", projection);
    ResMgr::GetShader("particle").Use().SetInteger("image", 0);
    ResMgr::GetShader("particle").SetMatrix4("projection", projection);

    // 设置渲染器
    m_Renderer = new Renderer(ResMgr::GetShader("shader"));
    // 加载纹理
    ResMgr::LoadTexture("res/textures/player.png", GL_TRUE, "player");
    ResMgr::LoadTexture("res/textures/ball.png", GL_TRUE, "ball");
    ResMgr::LoadTexture("res/textures/particle.png", GL_TRUE, "particle");
    ResMgr::LoadTexture("res/textures/background.jpg", GL_FALSE, "background");
    ResMgr::LoadTexture("res/textures/block.png", GL_FALSE, "block");
    ResMgr::LoadTexture("res/textures/block_solid.png", GL_FALSE, "block_solid");

    // 加载关卡
    GameLevel lvl_1; lvl_1.Load("res/levels/level_1", m_Width, m_Height * 0.5);
    GameLevel lvl_2; lvl_2.Load("res/levels/level_2", m_Width, m_Height * 0.5);
    GameLevel lvl_3; lvl_3.Load("res/levels/level_3", m_Width, m_Height * 0.5);
    GameLevel lvl_4; lvl_4.Load("res/levels/level_4", m_Width, m_Height * 0.5);

    m_Levels.push_back(lvl_1);
    m_Levels.push_back(lvl_2);
    m_Levels.push_back(lvl_3);
    m_Levels.push_back(lvl_4);
    m_Level = 0;

    // 初始化player
    glm::vec2 playerPos = glm::vec2(
            m_Width / 2 - PLAYER_SIZE.x / 2,
            m_Height - PLAYER_SIZE.y);
    m_Player = new GameObject(playerPos, PLAYER_SIZE, ResMgr::GetTexture("player"));

    // 初始化球
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    m_Ball = new BallObject(ballPos, BALL_RADIUS, BALL_VELOCITY, ResMgr::GetTexture("ball"));

    // 初始化粒子生成器
    m_PG = new ParticleGenerator(ResMgr::GetShader("particle"),
                                 ResMgr::GetTexture("particle"),
                                 500);
}

void Game::ResetLevel(){
    // 重置砖块
    for(GameObject &box: m_Levels[m_Level].m_Bricks){
        box.m_Destroyed = false;
    }
}

void Game::ResetPlayer(){
    // 重置玩家位置
    glm::vec2 playerPos = glm::vec2(
            m_Width / 2 - PLAYER_SIZE.x / 2,
            m_Height - PLAYER_SIZE.y);
    m_Player->m_Position = playerPos;
    // 重置球
    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    m_Ball->m_Position = ballPos;
    m_Ball->m_Stuck = true;
}

void Game::ProcessInput(GLfloat dt){
    // 游戏中
    if(m_State == GAME_ACTIVE){
        GLfloat velocity = PLAYER_VELOCITY * dt;
        // 移动挡板
        if(m_Keys[GLFW_KEY_A] || m_Keys[GLFW_KEY_LEFT]){
            if(m_Player->m_Position.x >= 0){
                m_Player->m_Position.x -= velocity;
                if(m_Ball->m_Stuck){
                    m_Ball->m_Position.x -= velocity;
                }
            }
        }
        if(m_Keys[GLFW_KEY_D] || m_Keys[GLFW_KEY_RIGHT]){
            if(m_Player->m_Position.x <= m_Width-m_Player->m_Size.x){
                m_Player->m_Position.x += velocity;
                if(m_Ball->m_Stuck){
                    m_Ball->m_Position.x += velocity;
                }
            }
        }
        if(m_Keys[GLFW_KEY_SPACE]){
            m_Ball->m_Stuck = false;
        }
    }
}

void Game::Update(GLfloat dt){
    m_Ball->Move(dt, m_Width);
    m_PG->Update(dt, *m_Ball, 2, glm::vec2(m_Ball->m_Radius / 2));
    DoCollisions();
    if(m_Ball->m_Position.y >= m_Height){  // 球是否触及底界
        ResetLevel();
        ResetPlayer();
    }
}

void Game::Render(){
    if(m_State == GAME_ACTIVE) {
        // 绘制背景
        m_Renderer->Draw(&ResMgr::GetTexture("background"),
                         glm::vec2(0, 0), glm::vec2(m_Width, m_Height),
                         0.0f);
        // 绘制关卡
        m_Levels[m_Level].Draw(*m_Renderer);

        // 绘制player
        m_Player->Draw(*m_Renderer);

        // 绘制粒子效果
        m_PG->Draw();

        // 绘制球
        m_Ball->Draw(*m_Renderer);
    }
}

void Game::DoCollisions() {
    // 砖块 - 球 检测
    for(GameObject &box: m_Levels[m_Level].m_Bricks){
        if(!box.m_Destroyed){
            Collision collision = CheckCollision(*m_Ball, box);
            if(std::get<0>(collision)){
                // 如果不是solid就撞毁方块
                if(!box.m_IsSolid){
                    box.m_Destroyed = GL_TRUE;
                }
                // 碰撞处理
                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);
                if(dir == LEFT || dir == RIGHT){ // 水平方向碰撞
                    m_Ball->m_Velocity.x = -m_Ball->m_Velocity.x; // 反转水平速度
                    // 重定位
                    GLfloat penetration = m_Ball->m_Radius - std::abs(diff_vector.x);
                    if(dir == LEFT){
                        m_Ball->m_Position.x += penetration; // 将球右移
                    } else {
                        m_Ball->m_Position.x -= penetration; // 将球左移
                    }
                } else { // 垂直方向碰撞
                    m_Ball->m_Velocity.y = -m_Ball->m_Velocity.y; // 反转垂直速度
                    // 重定位
                    GLfloat penetration = m_Ball->m_Radius - std::abs(diff_vector.y);
                    if(dir == UP){
                        m_Ball->m_Position.y -= penetration; // 将球下移
                    } else {
                        m_Ball->m_Position.y += penetration; // 将球上移
                    }
                }
            }
        }
    }
    // 玩家 - 球 检测
    Collision result = CheckCollision(*m_Ball, *m_Player);
    if(!m_Ball->m_Stuck && std::get<0>(result)){
        // 检查碰到了挡板的哪个位置
        GLfloat centerBoard = m_Player->m_Position.x + m_Player->m_Size.x / 2;
        GLfloat distance = (m_Ball->m_Position.x + m_Ball->m_Radius) - centerBoard;
        GLfloat percentage = distance / (m_Player->m_Size.x / 2);
        // 根据位置来改变球的速度
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = m_Ball->m_Velocity;
        m_Ball->m_Velocity.x = BALL_VELOCITY.x * percentage * strength;
//        m_Ball->m_Velocity.y = -m_Ball->m_Velocity.y;
        // 防止粘板现象 即球心进入AABB体内
        m_Ball->m_Velocity.y = -1 * abs(m_Ball->m_Velocity.y);
        m_Ball->m_Velocity = glm::normalize(m_Ball->m_Velocity) * glm::length(oldVelocity);
    }
}

GLboolean CheckCollision(GameObject &one, GameObject &two){
    // AABB - AABB 碰撞检测
    // x axis
    bool collisionX = one.m_Position.x + one.m_Size.x >= two.m_Position.x
                      && two.m_Position.x + two.m_Size.x >= one.m_Position.x;
    // y axis
    bool collisionY = one.m_Position.y + one.m_Size.y >= two.m_Position.y
                      && two.m_Position.y + two.m_Size.y >= one.m_Position.y;
    // 两轴都碰撞时才碰撞
    return collisionX && collisionY;
}

Collision CheckCollision(BallObject &one, GameObject &two){
    // AABB - 圆 碰撞检测
    // 获取圆心
    glm::vec2 center(one.m_Position + one.m_Radius);
    // 计算AABB信息(中心 半边长)
    glm::vec2 aabb_half_extents(two.m_Size.x / 2, two.m_Size.y / 2);
    glm::vec2 aabb_center(
            two.m_Position.x + aabb_half_extents.x,
            two.m_Position.y + aabb_half_extents.y
    );
    // 获取两个中心的矢量差
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
    glm::vec2 closest = aabb_center + clamped;
    // 获取圆心与closest的矢量差
    difference = closest - center;

    // 发生碰撞
    if(glm::length(difference) <= one.m_Radius){
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    } else {
        return std::make_tuple(GL_FALSE, UP, glm::vec2(0.0f));
    }
}

Direction VectorDirection(glm::vec2 target){
    glm::vec2 compass[] = {
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, -1.0f),
            glm::vec2(-1.0f, 0.0f),
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    // 找到与四个方向 夹角最小的方向 即为碰撞的点
    for(GLuint i = 0; i < 4; i++){
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if(dot_product > max){
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}
