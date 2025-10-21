#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "color.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

extern std::vector<Color> framebuffer;
extern Color clearColor;
extern Color currentColor;

void clear();
void point(int x, int y);
void renderBuffer(SDL_Renderer* renderer);