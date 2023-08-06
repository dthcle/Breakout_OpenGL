#ifndef INC_2D_GAME_LEARN_GAME_LEVEL_H
#define INC_2D_GAME_LEARN_GAME_LEVEL_H

#include <vector>
#include "game_object.h"

class GameLevel {
public:
    std::vector<GameObject> m_Bricks;

    GameLevel() {};
    void Load(const GLchar *DATA, GLuint levelWidth, GLuint levelHeight);
    void Draw(Renderer &renderer);
    GLboolean IsCompleted();
private:
    void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};


#endif //INC_2D_GAME_LEARN_GAME_LEVEL_H
