#include <fstream>
#include <sstream>

#include "game_level.h"
#include "res_mgr.h"

void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight) {
    // clear old data
    m_Bricks.clear();
    // load from file
    GLuint tileCode;
    GameLevel level;
    std::string line;
    std::ifstream input(file);
    std::vector<std::vector<GLuint>> tileData;

    if(input){
        while(std::getline(input, line)){
            std::istringstream inputStream(line);
            std::vector<GLuint> row;
            while(inputStream >> tileCode){
                row.push_back(tileCode);
            }
            tileData.push_back(row);
        }
        if(tileData.size() > 0){
            init(tileData, levelWidth, levelHeight);
        }
    }
}

void GameLevel::Draw(Renderer &renderer) {
    for(GameObject &tile: m_Bricks){
        if(!tile.m_Destroyed){
            tile.Draw(renderer);
        }
    }
}

GLboolean GameLevel::IsCompleted() {
    for(GameObject &tile: m_Bricks){
        if(!tile.m_IsSolid && !tile.m_Destroyed){
            return GL_FALSE;
        }
    }
    return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight) {
    // calculate dimensions
    GLuint row_n = tileData.size();
    GLuint column_n = tileData[0].size();
    // 计算单个砖块的大小
    GLfloat unit_width = levelWidth / static_cast<GLfloat>(column_n);
    GLfloat unit_height = levelHeight / row_n;

    for(GLuint y = 0; y < row_n; y++){
        for(GLuint x = 0; x < column_n; x++){
            glm::vec2 pos(unit_width*x, unit_height*y);
            glm::vec2 size(unit_width, unit_height);
            if(tileData[y][x] == 1) { // Solid
                GameObject obj(pos, size, ResMgr::GetTexture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
                obj.m_IsSolid = GL_TRUE;
                m_Bricks.push_back(obj);
            } else if (tileData[y][x] > 1){
                glm::vec3 color = glm::vec3(1.0f); // original: white
                switch (tileData[y][x]) {
                    case 2: color = glm::vec3(0.2f, 0.6f, 1.0f); break;
                    case 3: color = glm::vec3(0.0f, 0.7f, 0.0f); break;
                    case 4: color = glm::vec3(0.8f, 0.8f, 0.4f); break;
                    case 5: color = glm::vec3(1.0f, 0.5f, 0.0f); break;
                }
                m_Bricks.push_back(GameObject(pos, size, ResMgr::GetTexture("block"), color));
            }
        }
    }
}
