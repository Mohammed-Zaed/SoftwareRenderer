#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "display.h"
#include "SDL2/SDL.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* colorBufferTexture = NULL;

static bool isRunning = false;
uint32_t* colorBuffer = NULL;
uint32_t winWidth = 800;
uint32_t winHeight = 600;

void setup(void)
{
    isRunning = !initWindow();
    
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

void update(void) {
    drawGrid(10U, 10U, 0xFF00FF00);
    static uint32_t i = 0;
    static uint32_t j = 0;
    register uint32_t column = (i+10U) % winWidth;
    i = column;
    if (!column)
    {
        j = (j + 10U) % winHeight;
    }
    drawFillRectangle(j, i, 10, 10, 0xFFFFFF00);
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    clearColorBuffer(0x00);
    SDL_RenderClear(renderer);
    update();
    renderColorBuffer();
    SDL_RenderPresent(renderer);
}

int main(void)
{
    setup();
    while(isRunning) {
        processInput();
        render();
        renderColorBuffer();
    }
    destroyWindow();
    return 0;
}