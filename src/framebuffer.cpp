#include "framebuffer.h"
#include <algorithm>

std::vector<Color> framebuffer(SCREEN_WIDTH * SCREEN_HEIGHT);
Color clearColor(0, 0, 0);
Color currentColor(255, 255, 255);

void clear() {
    std::fill(framebuffer.begin(), framebuffer.end(), clearColor);
}

void point(int x, int y) {
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        framebuffer[y * SCREEN_WIDTH + x] = currentColor;
    }
}

void renderBuffer(SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        SCREEN_WIDTH,
        SCREEN_HEIGHT
    );

    void* pixels;
    int pitch;
    SDL_LockTexture(texture, nullptr, &pixels, &pitch);

    uint32_t* pixelData = static_cast<uint32_t*>(pixels);
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
        Color& color = framebuffer[i];
        pixelData[i] = (color.a << 24) | (color.b << 16) | (color.g << 8) | color.r;
    }

    SDL_UnlockTexture(texture);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}