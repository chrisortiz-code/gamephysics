#ifndef BALL_H
#define BALL_H

#include <SDL.h>

class Ball {
public:
    Ball();
    Ball(int level, SDL_Renderer* renderer);
    void draw();
    void update();
    void startDrag(int mouseX, int mouseY);
    void updateDrag(int mouseX, int mouseY);
    void shoot();
    void drawProjectionLine();
    [[nodiscard]] SDL_Rect getBounds() const;
    void reverseVelocity(int side);
    int velocityY;
private:
    int centerX;
    int centerY;
    SDL_Renderer* renderer;
    static const int radius = 10;
    static const int maxVelocity = 6;       // Maximum velocity
    static const int projectionLineLength = 100;

    int dragStartX;
    int dragStartY;
    int dragEndX;
    int dragEndY;
    bool isDragging;
    int velocityX;

};

#endif // BALL_H