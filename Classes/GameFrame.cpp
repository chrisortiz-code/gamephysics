#include "GameFrame.h"
#include "Walls.h"
#include "Ball.h"
#include "LineObstacle.h"
#include <algorithm>
#include <initializer_list>

enum class CollisionSide;

GameFrame::GameFrame(int level, SDL_Window *window, SDL_Renderer *renderer)
    : window(window), renderer(renderer), ball(level, renderer), gameWalls(level, renderer),
      isDragging(false), gameOver(false) {
    // Set dimensions based on the level
    switch (level) {
        case 0:
            width = 800;
            height = 600;
            obstacles.push_back(LineObstacle(renderer, 0.5f, 0, 100, 150)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.3f, 1, 300, 200)); // Vertical obstacle
            break;

        case 1:
            width = 1024;
            height = 768;
            obstacles.push_back(LineObstacle(renderer, 0.6f, 0, 200, 250)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.4f, 1, 400, 300)); // Vertical obstacle
            break;

        case 2:
            width = 1280;
            height = 720;
            obstacles.push_back(LineObstacle(renderer, 0.7f, 0, 250, 350)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.5f, 1, 500, 400)); // Vertical obstacle
            break;

        case 3:
            width = 1366;
            height = 768;
            obstacles.push_back(LineObstacle(renderer, 0.8f, 0, 300, 400)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.6f, 1, 600, 450)); // Vertical obstacle
            break;

        case 4:
            width = 1920;
            height = 1080;
            obstacles.push_back(LineObstacle(renderer, 0.9f, 0, 350, 500)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.7f, 1, 700, 550)); // Vertical obstacle
            break;

        default:
            width = 800;
            height = 600;
            obstacles.push_back(LineObstacle(renderer, 0.5f, 0, 100, 150)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.3f, 1, 300, 200)); // Vertical obstacle
            break;
    }

    walls = gameWalls.getBoundsArray(); // Load walls as an array of SDL_Rects
}



GameFrame::~GameFrame() {
}

void GameFrame::handleEvents(SDL_Event &e) {
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        SDL_Rect ballBounds = ball.getBounds();
        if (mouseX >= ballBounds.x && mouseX < ballBounds.x + ballBounds.w &&
            mouseY >= ballBounds.y && mouseY < ballBounds.y + ballBounds.h) {
            ball.startDrag(mouseX, mouseY);
            isDragging = true;
        }
    } else if (e.type == SDL_MOUSEMOTION && isDragging) {
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        ball.updateDrag(mouseX, mouseY);
    } else if (e.type == SDL_MOUSEBUTTONUP && isDragging) {
        ball.shoot();
        isDragging = false;
    }
}

void GameFrame::update() {
    if (!gameOver) {
        checkCollisions();
        ball.update();
        if (isBallOutside()) {
            gameOver = true;
        }
    }
}

void GameFrame::render() {
    if (!gameOver) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        gameWalls.draw();
        ball.draw();
        for (LineObstacle &obstacle: obstacles) {
            obstacle.draw();
        }
        if (isDragging) {
            ball.drawProjectionLine();
        }

        SDL_RenderPresent(renderer);
    }
}int GameFrame::detectCollisionSide(SDL_Rect &ball, SDL_Rect &obstacle) {
    int leftPen = ball.x + ball.w - obstacle.x;
    int rightPen = obstacle.x + obstacle.w - ball.x;
    int topPen = ball.y + ball.h - obstacle.y;
    int bottomPen = obstacle.y + obstacle.h - ball.y;

    int minPen = std::min({leftPen, rightPen, topPen, bottomPen});

    if (minPen == topPen) return 0;
    if (minPen == rightPen) return 1;
    if (minPen == bottomPen) return 2;
    return 3;
}

void GameFrame::checkCollisions() {
    SDL_Rect ballBounds = ball.getBounds();
    const std::vector<SDL_Rect> &wallBounds = gameWalls.getBoundsArray();

    for (size_t i = 0; i < wallBounds.size(); i++) {
        SDL_Rect currentWall = wallBounds[i];
        if (SDL_HasIntersection(&ballBounds, &currentWall)) {
            int side = detectCollisionSide(ballBounds,currentWall);


            //used to combat ball shooting in opposite direction when hitting wall, while already in the exit
            if(SDL_HasIntersection(&ballBounds,&gameWalls.exitHole)) {
                ball.reverseVelocity(gameWalls.exitSide-1);
            }else {
                ball.reverseVelocity(side);
            }
        }
    }
    for (size_t i = 0; i < obstacles.size(); i++) {
        SDL_Rect currentObstacle = obstacles[i].getBounds();
        if (SDL_HasIntersection(&ballBounds, &currentObstacle)) {
            int side = detectCollisionSide(ballBounds, currentObstacle);
            ball.reverseVelocity(side);
        }
    }

    if (SDL_HasIntersection(&ballBounds, &gameWalls.extraWall)) {
        switch (gameWalls.exitSide%2) {
            case 0:
                ball.reverseVelocity(gameWalls.exitSide);
                break;
            case 1:
                ball.reverseVelocity(2);
                break;
        }
    }
}

bool GameFrame::isBallOutside() const {
    SDL_Rect ballBounds = ball.getBounds();
    return (ballBounds.x + ballBounds.w < 0 ||
            ballBounds.x > width ||
            ballBounds.y + ballBounds.h < 0 ||
            ballBounds.y > height);
}
