#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include "SDL2/SDL.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* colorBufferTexture = NULL;

bool isRunning = false;
uint32_t* colorBuffer = NULL;
uint32_t winWidth = 800;
uint32_t winHeight = 600;

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
    return result;
}

void setup(void)
{
    isRunning = !initWindow();
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    
    colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * winWidth * winHeight);
    if (!colorBuffer) {
        fprintf(stderr, "Error:: Memory allocation for color buffer failed.\n");
    }
    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        winWidth,
        winHeight
    );
}

void update(void)
{
}

void processInput(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch(event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
            
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            break;
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
    if (result)
    {
        const char* errorMessage = SDL_GetError();
        fprintf(stderr, "Error:: %s", errorMessage);
        assert(errorMessage);
    }
    SDL_RenderCopy(renderer, colorBufferTexture, NULL, NULL);
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    renderColorBuffer();
    clearColorBuffer(0xFFFFFF00);
    SDL_RenderPresent(renderer);
}

void destroy(void)
{
    free(colorBuffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(void)
{
    setup();
    while(isRunning) {
        processInput();
        update();
        render();
        renderColorBuffer();
    }
    destroy();
    return 0;
}
