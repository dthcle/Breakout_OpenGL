set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}../)


add_executable(test WIN32
        main.cpp
        glad.cpp stb_image.cpp

        common.h

        game.cpp    game.h
        shader.cpp  shader.h
        texture.cpp texture.h
        res_mgr.cpp res_mgr.h
        render.cpp  render.h
        game_object.cpp game_object.h
        game_level.cpp game_level.h
        particle_gen.cpp particle_gen.h
)
