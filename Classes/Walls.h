/*
     *Used to easily create levels using the following parameters
        *sf Vector dimensions
        *int exitSide
        *float(2) exitProportion
     *public
        *sf Vector[4] getBounds
        *sf Vector getExitBounds
        *
     *private

        *int thickness
        *
*/
#ifndef WALLS_H
#define WALLS_H

#include <SDL.h>
#include <vector>

class Walls {
public:
   Walls(int level, SDL_Renderer* renderer);
   const std::vector<SDL_Rect>& getBoundsArray() const;
   void draw() const;
   SDL_Rect extraWall;
   int exitSide;
   SDL_Rect exitHole;
private:
   SDL_Renderer* renderer;
   std::vector<SDL_Rect> walls;

   int height;
   int length;
   float exitX;



   void createWalls();

};

#endif // WALLS_H



