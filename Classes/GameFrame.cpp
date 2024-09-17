#include "GameFrame.h"
#include "Walls.h"
#include "Ball.h"
#include "LineObstacle.h"


enum class CollisionSide;

GameFrame::GameFrame(int level, SDL_Window *window, SDL_Renderer *renderer)
    : window(window), renderer(renderer), ball(level, renderer), gameWalls(level, renderer),
      isDragging(false), gameOver(false) {
    // Set dimensions based on the level
    switch (level) {
        case 0:
            width = 800;
            height = 600;
        // Load obstacles for level 0
            obstacles.push_back(LineObstacle(renderer, 0.5f, 0, 100, 150)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.3f, 1, 300, 200)); // Vertical obstacle
            break;

        case 1:
            width = 1024;
            height = 768;
        // Load obstacles for level 1
            obstacles.push_back(LineObstacle(renderer, 0.6f, 0, 200, 250)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.4f, 1, 400, 300)); // Vertical obstacle
            break;

        case 2:
            width = 1280;
            height = 720;
        // Load obstacles for level 2
            obstacles.push_back(LineObstacle(renderer, 0.7f, 0, 250, 350)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.5f, 1, 500, 400)); // Vertical obstacle
            break;

        case 3:
            width = 1366;
            height = 768;
        // Load obstacles for level 3
            obstacles.push_back(LineObstacle(renderer, 0.8f, 0, 300, 400)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.6f, 1, 600, 450)); // Vertical obstacle
            break;

        case 4:
            width = 1920;
            height = 1080;
        // Load obstacles for level 4
            obstacles.push_back(LineObstacle(renderer, 0.9f, 0, 350, 500)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.7f, 1, 700, 550)); // Vertical obstacle
            break;

        default:
            width = 800;
            height = 600;
        // Load default obstacles
            obstacles.push_back(LineObstacle(renderer, 0.5f, 0, 100, 150)); // Horizontal obstacle
            obstacles.push_back(LineObstacle(renderer, 0.3f, 1, 300, 200)); // Vertical obstacle
            break;
    }

    walls = gameWalls.getBoundsArray(); // Load walls as an array of SDL_Rects
}

int GameFrame::detectCollisionSide(SDL_Rect &ball,SDL_Rect &obstacle) {
    int ballCenterX = ball.x + ball.w / 2;
    int ballCenterY = ball.y + ball.h / 2;

    int obstacleCenterX = obstacle.x + obstacle.w / 2;
    int obstacleCenterY = obstacle.y + obstacle.h / 2;

    int dx = obstacleCenterX - ballCenterX;
    int dy = obstacleCenterY - ballCenterY;

    if (std::abs(dx) > std::abs(dy)) {
        return dx > 0 ? 3 : 4;
    }
    return dy > 0 ? 1 : 2;
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
}

void GameFrame::checkCollisions() {
    SDL_Rect ballBounds = ball.getBounds();
    const std::vector<SDL_Rect> &wallBounds = gameWalls.getBoundsArray();

    for (size_t i = 0; i < wallBounds.size(); i++) {
        SDL_Rect currentWall = wallBounds[i];
        if (SDL_HasIntersection(&ballBounds, &currentWall)) {
             int side = detectCollisionSide(ballBounds,currentWall);
            SDL_Log("Collision detected with wall %zu", i);
            SDL_Log("Side %zu",side);
            ball.reverseVelocity(i);
        }
    }

    if (SDL_HasIntersection(&ballBounds, &gameWalls.extraWall)) {
        switch (gameWalls.exitSide) {
            case 0:
            case 1:
                ball.reverseVelocity(0);
                break;
            case 2:
            case 3:
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
