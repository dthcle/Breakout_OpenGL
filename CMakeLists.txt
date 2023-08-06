cmake_minimum_required(VERSION 3.26)
project(Breakout_OpenGL)

set(CMAKE_CXX_STANDARD 17)

# 判断当前是release模式还是debug模式(默认是release模式)
if(CMAKE_BUILD_TYPE)
    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
    if(${BUILD_TYPE} STREQUAL "release")
        set(RELEASE 1)
    endif()
else ()
    set(RELEASE)
endif ()
if(RELEASE)
    message("-- Current build type is RELEASE")
else()
    message("-- Current build type is DEBUG")
    add_definitions(-D__DEBUG__)
endif ()

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    link_directories(${OPENGL_LIBRARY_DIRS})
    add_definitions(${OPENGL_DEFINITIONS})
else()
    message(ERROR "!!! OpenGL library not found !!!")
endif()

link_directories(lib)
include_directories(include)

add_subdirectory(src)

target_link_libraries(test
        libglfw3.a
        OpenGL::GL
)

# 将资源文件嵌入
# 需要生成嵌入的文件
# file(GLOB var path) 将path下所有文件的路径赋值给var
# file(GLOB_RECURSE var path) 如果path中的文件包含了文件夹则递归搜索
file(GLOB_RECURSE EMBED_FILES "${CMAKE_CURRENT_SOURCE_DIR}/res/*")

# 输出文件目录
#set(OUTPUT_HDR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")
#set(OUTPUT_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/")

# 输出文件名
set(OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
# 因为Clion同时生成release和debug时 会导致文件内容被写入两次 出现重定义的错误
if(RELEASE)
    set(OUT_FILENAME "${OUT_DIR}/resource")
else()
    set(OUT_FILENAME "${OUT_DIR}/resource_d")
endif ()
# 清空输出文件
file(WRITE ${OUT_FILENAME}.c "")
file(WRITE ${OUT_FILENAME}.h "")

# 依次处理文件
foreach(input_src ${EMBED_FILES})
    message("Add Resource File: ${input_src}")
    # 配置输出文件名
    file(SIZE ${input_src} embed_file_size)
    get_filename_component(embed_file_name ${input_src} NAME)
    #    set(gen_embed_file          "${OUTPUT_SRC_DIR}/${embed_file}.c")
    #    set(gen_embed_file_header   "${OUTPUT_HDR_DIR}/${embed_file}.h")
    # 清空输出文件
    #    file(WRITE ${gen_embed_file} "")
    #    file(WRITE ${gen_embed_file_header} "")
    # for c compatibility
    string(MAKE_C_IDENTIFIER ${embed_file_name} token)
    # to upper case
    string(TOUPPER ${token} token)
    # read hex data from file
    file(READ ${input_src} file_data HEX)
    # convert hex data for C compatibility
    string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," file_data ${file_data})
    # append data to output file
    file(APPEND ${OUT_FILENAME}.c
            "const unsigned char ${token}__DATA[] = {\n${file_data}0x00\n};\n"
            "const unsigned long ${token}__SIZE   = ${embed_file_size};\n")
    file(APPEND ${OUT_FILENAME}.h
            "extern const unsigned char ${token}__DATA[];\n"
            "extern const unsigned long ${token}__SIZE;\n")
    # 加入到生成文件列表
    #    list(APPEND GEN_EMBED_FILES
    #    ${gen_embed_file}
    #    ${gen_embed_file_header})
endforeach ()

## 将生成的文件添加到embed_gen_files中
#add_custom_target(
#        embed_gen_files
#        DEPENDS ${GEN_EMBED_FILES}
#)
# 将生成的头文件目录添加到include中
#include_directories(${OUTPUT_HDR_DIR})
#add_dependencies(${PROJECT_NAME} embed_gen_files)

# 将生成的文件添加到executable中去
target_sources(test PRIVATE ${OUT_FILENAME}.c ${OUT_FILENAME}.h)
