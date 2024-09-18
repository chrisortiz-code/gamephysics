#include "Ball.h"
#include <cmath>
#include <iostream>

Ball::Ball(int level, SDL_Renderer *renderer)
    : renderer(renderer), isDragging(false), velocityX(0), velocityY(0) {
    switch (level) {
        case 0:
            // Level 0 - 800x600, exit is at 30% width on top side
            centerX = 600; // Positioned far from the exit (closer to the right side)
            centerY = 500; // Lower part of the frame

            break;

        case 1:
            // Level 1 - 1024x768, exit is on the right side at 50% height
            centerX = 300; // Positioned on the far left
            centerY = 600; // Lower part of the frame
            break;

        case 2:
            // Level 2 - 1280x720, exit is on the bottom side at 70% width
            centerX = 200; // Positioned on the far left
            centerY = 100; // Positioned at the top (far from the bottom exit)
            break;

        case 3:
            // Level 3 - 1366x768, exit is on the left side at 40% height
            centerX = 1000; // Positioned far right
            centerY = 600; // Lower part of the frame
            break;

        case 4:
            // Level 4 - 1920x1080, exit is on the top side at 60% width
            centerX = 300; // Positioned on the far left
            centerY = 900; // Lower part of the frame
            break;

        default:
            // Default - 800x600 fallback
            centerX = 600; // Positioned far from the middle
            centerY = 500; // Lower part of the frame
            break;
    }
}

void Ball::draw() {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

void Ball::update() {
    centerX += velocityX;
    centerY += velocityY;
}

void Ball::startDrag(int mouseX, int mouseY) {
    isDragging = true;
    dragStartX = centerX;
    dragStartY = centerY;
    dragEndX = mouseX;
    dragEndY = mouseY;
}

void Ball::updateDrag(int mouseX, int mouseY) {
    if (isDragging) {
        dragEndX = mouseX;
        dragEndY = mouseY;
    }
}

void Ball::shoot() {
    if (isDragging) {
        int dx = dragStartX - dragEndX;
        int dy = dragStartY - dragEndY;

        double totalVelocitySquared = dx * dx + dy * dy;
        double maxVelocitySquared = maxVelocity * maxVelocity;

        if (totalVelocitySquared > maxVelocitySquared) {
            double scale = std::sqrt(maxVelocitySquared / totalVelocitySquared);
            dx = static_cast<int>(dx * scale);
            dy = static_cast<int>(dy * scale);
        }

        velocityX = dx;
        velocityY = dy;

        isDragging = false;
    }
}

void Ball::drawProjectionLine() {
    if (isDragging) {
        int dx = centerX - dragEndX;
        int dy = centerY - dragEndY;

        double distance = std::sqrt(dx * dx + dy * dy);



        const int dashLength = 10;
        const int dashInterval =15;
        const int maxDashes = 5;

        int numDashes = std::min(static_cast<int>(distance / dashInterval), maxDashes);

        double unitX = dx / distance;
        double unitY = dy / distance;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int i = 0; i < numDashes; ++i) {
            int startX = static_cast<int>(centerX + i * dashInterval * unitX);
            int startY = static_cast<int>(centerY + i * dashInterval * unitY);
            int endX = static_cast<int>(startX + dashLength * unitX);
            int endY = static_cast<int>(startY + dashLength * unitY);

            SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
        }
    }
}

SDL_Rect Ball::getBounds() const {
    return SDL_Rect{centerX - radius, centerY - radius, radius * 2, radius * 2};
}

void Ball::reverseVelocity(int side) {
    switch (side%2) {
        case 0:velocityY *= -1;break;
        case 1:velocityX *= -1;break;
    }
}
