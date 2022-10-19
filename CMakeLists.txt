cmake_minimum_required(VERSION 3.23)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
include(FetchContent)


FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8)
FetchContent_MakeAvailable(glm)

add_executable(untitled sphere_importance.cpp)

FetchContent_Declare(stb
        GIT_REPOSITORY https://github.com/nothings/stb
        GIT_TAG 8b5f1f37b5b75829fc72d38e7b5d4bcbf8a26d55)
FetchContent_MakeAvailable(stb)
add_library(stb INTERFACE ${stb_SOURCE_DIR}/stb_image.h)
target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE glm)
