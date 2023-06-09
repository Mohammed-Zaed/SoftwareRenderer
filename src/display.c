#include <stdbool.h>
#include <ctype.h>
#include "SDL2/SDL.h"
#include "display.h"


extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Texture* colorBufferTexture;

extern uint32_t* colorBuffer;
extern float* zBuffer;
extern uint32_t winWidth;
extern uint32_t winHeight;

uint8_t initWindow(void)
{
    uint8_t result = SDL_Init(SDL_INIT_EVERYTHING);
    if (result == 0) {
        printf("SDL Initialisation was sucessfull. \n");
    } else {
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
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void drawPixel(uint32_t x, uint32_t y, uint32_t color) {
    if ((x < winWidth) && (y < winHeight)) {
        colorBuffer[(winWidth * y) + x] = color;
    } else {
        static uint32_t once = 1;
        if (once) {
            fprintf(stderr, "Error:: Pixel x or y out of boundary\n");
            once = 0;
        }
    }
}

void drawFillRectangle(uint32_t x, uint32_t y, uint32_t l1, uint32_t l2, uint32_t color) {
    for (uint32_t i = y; i < (l1 + y); ++i) {
        for (uint32_t j = x; j < (l2 + x); ++j) {
            drawPixel(j, i, color);
        }
    }
}

void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color) {
    int32_t deltaX = x1 - x0;
    int32_t deltaY = y1 - y0;

    uint32_t longestSideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);
    
    float xInc = (float)deltaX / (float)longestSideLength;
    float yInc = (float)deltaY / (float)longestSideLength;
    
    float currX = x0;
    float currY = y0;

    for (uint32_t i = 0U; i <= longestSideLength; ++i) {
        drawPixel(round(currX), round(currY), color);
        currX += xInc;
        currY += yInc;
    }
}

void clearZBuffer(void) {
    for (uint32_t y = 0U; y < winHeight; ++y) {
        for (uint32_t x = 0U; x < winWidth; ++x) {
            zBuffer[(winWidth * y) + x] = 1.0F;
        }
    }
}