#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#include "display.h"
#include "vector.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* colorBufferTexture = NULL;

uint32_t winWidth = 800;
uint32_t winHeight = 600;

uint32_t* colorBuffer = NULL;
static bool isRunning = false;

const uint32_t VECTORS_FOR_EACH_AXIS = 9U;
const uint32_t NUM_OF_VECTORS_CUBE = VECTORS_FOR_EACH_AXIS * VECTORS_FOR_EACH_AXIS * VECTORS_FOR_EACH_AXIS;
const uint32_t LENGTH_OF_CUBE = 2U;

const float fovFactor = 128.00F;
vec3_t cube[NUM_OF_VECTORS_CUBE] = {0};


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

    const float delta = ((float)LENGTH_OF_CUBE / (float)VECTORS_FOR_EACH_AXIS) + 0.005F;
    uint32_t currentIndex = 0U;

    for (float x = -1.0; x <= 1.0F; x +=  delta) {
        for (float y = -1.0; y <= 1.0F; y += delta) {
            for (float z = -1.0; z <= 1.0F; z += delta) {
                vec3_t point = { .x = x, .y = y, .z = z};
                cube[currentIndex++] = point;
            }
        }
    }
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

vec2_t project(vec3_t point) {
    vec2_t projectedPoint = {
        fovFactor * point.x,
        fovFactor * point.y
    };
    return projectedPoint;
}

void update(void) {

    for (uint32_t i = 0U; i < NUM_OF_VECTORS_CUBE; ++i) {
        vec2_t projectedVector = project(cube[i]);
        drawFillRectangle(
                projectedVector.x + (winWidth / 2U),
                projectedVector.y + (winHeight / 2U),
                5U,
                5U,
                0x0000FF00
        );
        drawPixel(
                projectedVector.x + (winWidth / 2U),
                projectedVector.y + (winHeight / 2U),
                0xFFFF0000
        );

    }
}

void render(void)
{
    SDL_RenderClear(renderer);
    clearColorBuffer(0x00);
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