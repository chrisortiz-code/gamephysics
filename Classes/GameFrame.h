#ifndef GAMEFRAME_H
#define GAMEFRAME_H

#include <SDL.h>
#include "Ball.h"
#include "LineObstacle.h"
#include "Walls.h"

class GameFrame {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Ball ball;
    Walls gameWalls;
    int width;
    int height;
    bool isDragging;
    bool gameOver;
    std::vector<LineObstacle> obstacles;
    std::vector<SDL_Rect> walls;
    void checkCollisions();
    bool isBallOutside() const;
public:
    GameFrame(int level, SDL_Window* window, SDL_Renderer* renderer);
    ~GameFrame();
    int detectCollisionSide(SDL_Rect& ball,SDL_Rect& obstacle);
    void handleEvents(SDL_Event& e);
    void update();
    void render();
    bool isGameOver() const { return gameOver; }

};

#endif // GAMEFRAME_H