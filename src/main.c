#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include "SDL2/SDL.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

bool isRunning = false;

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
    
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
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
}

void update(void)
{
    //TODO::
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

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main(void)
{
    setup();

    while(isRunning) {
        processInput();
        update();
        render();
    }
    return 0;
}
