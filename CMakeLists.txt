cmake_minimum_required(VERSION 3.23)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
include(FetchContent)


FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 0.9.9.8)
FetchContent_MakeAvailable(glm)

add_executable(untitled main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glm)
