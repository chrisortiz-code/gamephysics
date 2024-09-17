#include "LineObstacle.h"

LineObstacle::LineObstacle(SDL_Renderer* renderer, float lengthRatio, int rotation, int posX, int posY)
    : renderer(renderer), lengthRatio(lengthRatio), rotation(rotation), posX(posX), posY(posY) {
    createObstacle();
}

void LineObstacle::createObstacle() {
    if (rotation == 0) {  // Horizontal
        rect.w = static_cast<int>(lengthRatio * 250);  // Assuming a max length of 1000 pixels
        rect.h = 20;  // Fixed height for horizontal lines
        rect.x = posX;
        rect.y = posY;
    } else {  // Vertical
        rect.w = 20;  // Fixed width for vertical lines
        rect.h = static_cast<int>(lengthRatio * 250);  // Assuming a max length of 1000 pixels
        rect.x = posX;
        rect.y = posY;
    }
}

void LineObstacle::draw() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Red color for obstacles
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect LineObstacle::getBounds() {
    return rect;
}
