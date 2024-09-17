#ifndef LINEOBSTACLE_H
#define LINEOBSTACLE_H

#include <SDL.h>

class LineObstacle {
public:
    LineObstacle(SDL_Renderer* renderer, float lengthRatio, int rotation, int posX, int posY);
    void createObstacle();
    void draw();
    SDL_Rect getBounds();

private:
    SDL_Renderer* renderer;
    SDL_Rect rect;
    float lengthRatio;  // Proportional length to a standard max length
    int rotation;       // 0 for horizontal, 1 for vertical
    int posX;           // X position in pixels
    int posY;           // Y position in pixels
};

#endif // LINEOBSTACLE_H
