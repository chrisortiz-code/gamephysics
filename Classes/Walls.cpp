#include "Walls.h"

#include <iostream>


Walls::Walls(int level, SDL_Renderer *renderer)
    : renderer(renderer) {
    switch (level) {
        case 0:
            length = 800;
            height = 600; // Half of the frame height for this level
            exitSide = 0; // Exit on the top side
            exitX = 0.3f; // 30% of the width
            break;

        case 1:
            length = 1024;
            height = 768; // Half of the frame height for this level
            exitSide = 1; // Exit on the right side
            exitX = 0.5f; // 50% down the height
            break;

        case 2:
            length = 1280;
            height = 720; // Half of the frame height for this level
            exitSide = 2; // Exit on the bottom side
            exitX = 0.7f; // 70% along the width
            break;

        case 3:
            length = 1366;
            height = 768; // Half of the frame height for this level
            exitSide = 3; // Exit on the left side
            exitX = 0.4f; // 40% down the height
            break;

        case 4:
            length = 1920;
            height = 1080; // Half of the frame height for this level
            exitSide = 0; // Exit on the top side
            exitX = 0.6f; // 60% along the width
            break;

        default:
            length = 800;
            height = 600; // Default length and height for fallback
            exitSide = 2; // Exit on the bottom side
            exitX = 0.5f; // 50% along the width
            break;
    }
    createWalls();
}

void Walls::createWalls() {
    int borderThickness = 20;
    int padding = 20; // Add some padding around the walls
    int exitSize = 100;

    SDL_Rect topWall, bottomWall, leftWall, rightWall;

    int wallWidth = length - 2 * padding;
    int wallHeight = height - 2 * padding;

    switch (exitSide) {
        case 0: // Top
            topWall = {padding, padding, wallWidth * exitX, borderThickness}; // Shortened wall
            bottomWall = {padding, height - padding - borderThickness, wallWidth, borderThickness};
            leftWall = {padding, padding, borderThickness, wallHeight};
            rightWall = {length - padding - borderThickness, padding, borderThickness, wallHeight};

            extraWall = {
                padding + wallWidth * exitX + exitSize, padding, wallWidth * (1 - exitX) - exitSize, borderThickness
            };

            exitHole = {
                static_cast<int>(padding + wallWidth * exitX),
                padding,
                exitSize,
                borderThickness
            };
            break;
        case 1: // Right
            topWall = {padding, padding, wallWidth, borderThickness};
            bottomWall = {padding, height - padding - borderThickness, wallWidth, borderThickness};
            leftWall = {padding, padding, borderThickness, wallHeight};
            rightWall = {
                length - padding - borderThickness, padding, borderThickness, wallHeight * exitX
            }; // Shortened wall

            extraWall = {
                length - padding - borderThickness, padding + wallHeight * exitX + exitSize, borderThickness,
                wallHeight * (1 - exitX) - exitSize
            };

            exitHole = {
                length - padding - borderThickness,
                static_cast<int>(padding + wallHeight * exitX),
                borderThickness,
                exitSize
            };
            break;
        case 2: // Bottom
            topWall = {padding, padding, wallWidth, borderThickness};
            bottomWall = {
                padding, height - padding - borderThickness, wallWidth * exitX, borderThickness
            }; // Shortened wall
            leftWall = {padding, padding, borderThickness, wallHeight};
            rightWall = {length - padding - borderThickness, padding, borderThickness, wallHeight};

            extraWall = {
                padding + wallWidth * exitX + exitSize, height - padding - borderThickness,
                wallWidth * (1 - exitX) - exitSize, borderThickness
            };

            exitHole = {
                static_cast<int>(padding + wallWidth * exitX),
                height - padding - borderThickness,
                exitSize,
                borderThickness
            };
            break;

        case 3: // Left
            topWall = {padding, padding, wallWidth, borderThickness};
            bottomWall = {padding, height - padding - borderThickness, wallWidth, borderThickness};
            leftWall = {padding, padding, borderThickness, wallHeight * exitX}; // Shortened wall
            rightWall = {length - padding - borderThickness, padding, borderThickness, wallHeight};

            extraWall = {
                padding, padding + wallHeight * exitX + exitSize, borderThickness, wallHeight * (1 - exitX) - exitSize
            };

            exitHole = {
                padding,
                static_cast<int>(padding + wallHeight * exitX),
                borderThickness,
                exitSize
            };
            break;
    }

    walls.push_back(topWall);
    walls.push_back(rightWall);
    walls.push_back(bottomWall);
    walls.push_back(leftWall);

    //walls.push_back(extraWall);
}

void Walls::draw() const {
    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);

    for (const auto &wall: walls) {
        SDL_RenderFillRect(renderer, &wall);
    }
    SDL_RenderFillRect(renderer, &extraWall);

    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 50, 0, 255);

    SDL_RenderPresent(renderer);
    SDL_RenderFillRect(renderer, &exitHole);
}

const std::vector<SDL_Rect> &Walls::getBoundsArray() const {
    return walls;
}
