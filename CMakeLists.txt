cmake_minimum_required(VERSION 3.10)

project(physics)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH  ${CMAKE_SOURCE_DIR}/Modules)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SDL2_PATH "C:/SDL2/x86_64-w64-mingw32")
find_package(SDL2 REQUIRED)
# Add the executable
add_executable(physics
        main.cpp
        main2.cpp
        Classes/GameFrame.cpp
        Classes/Walls.cpp
        Classes/Ball.cpp
        Classes/Obstacle.cpp
        Classes/GameFrame.h
        Classes/Walls.h
        main2.cpp
        Classes/LineObstacle.cpp
        Classes/LineObstacle.h
)

# Include SDL2 directories
include_directories(${SDL2_INCLUDE_DIR})

# Link SDL2 libraries
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
