#ifndef CIRCLE_H
#define CIRCLE_H

#include <SDL2/SDL.h>
int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
#endif
