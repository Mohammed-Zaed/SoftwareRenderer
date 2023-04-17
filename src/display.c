#include <stdbool.h>
#include <ctype.h>
#include "SDL2/SDL.h"
#include "display.h"


extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* colorBufferTexture;

extern uint32_t* colorBuffer;
extern uint32_t winWidth;
extern uint32_t winHeight;

uint8_t initWindow(void)
{
    uint8_t result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result == 0)
    {
        printf("SDL Initialisation was sucessfull. \n");
    }
    else
    {
        fprintf(stderr, "Error Initialising window.\n");
    }
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
    winWidth = displayMode.w;
    winHeight = displayMode.h;

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        winWidth,
        winHeight,
        SDL_WINDOW_BORDERLESS
    );
    if(!window)
    {
        fprintf(stderr, "Error:: Creating sdl window");
        result = 1;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
    {
        fprintf(stderr, "Error:: Creating sdl renderer");
        result = 2;
    }
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    return result;
}

void drawGrid(uint32_t deltaX, uint32_t deltaY, uint32_t color) {

    for (uint32_t y = 0; y < winWidth; y += deltaY) {
        for (uint32_t x = 0; x < winHeight; ++x) {
            colorBuffer[(winWidth * x) + y] = color;
        }
    }

    for (uint32_t y = 0; y < winWidth; ++y) {
        for (uint32_t x = 0; x < winHeight; x += deltaX) {
            colorBuffer[(winWidth * x) + y] = color;
        }
    }
}

void clearColorBuffer(uint32_t color) {
    for (uint32_t y = 0U; y < winHeight; ++y) {
        for (uint32_t x = 0U; x < winWidth; ++x) {
            colorBuffer[(winWidth * y) + x] = color;
        }
    }
}

void renderColorBuffer(void)
{
    int32_t result = SDL_UpdateTexture(
                        colorBufferTexture,
                        NULL,
                        colorBuffer,
                        winWidth * sizeof(uint32_t)
                     ); 
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}

void destroyWindow(void)
{
    free(colorBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void drawFillRectangle(uint32_t x, uint32_t y, uint32_t l1, uint32_t l2, uint32_t color) {
    for (uint32_t i = y; i < (l1 + y); ++i) {
        for (uint32_t j = x; j < (l2 + x); ++j) {
            colorBuffer[(winWidth  * j) + i] = color;
        }
    }
}
void drawPixel(uint32_t x, uint32_t y, uint32_t color) {
    if ((x < winWidth) && (y < winHeight)) {
        colorBuffer[(winWidth * y) + x] = color;
    }
    else
    {
        fprintf(stderr, "Error:: Pixel x or y out of boundary\n");
    }

}