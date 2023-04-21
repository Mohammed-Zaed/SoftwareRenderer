#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#include "display.h"
#include "vector.h"
#include "triangle.h"
#include "mesh.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* colorBufferTexture = NULL;

uint32_t winWidth = 800;
uint32_t winHeight = 600;

uint32_t* colorBuffer = NULL;
const uint32_t FPS = 33;
static bool isRunning = false;


const uint32_t fovFactor = 800.00F;

vec3_t cameraPosition = {0.0, 0.0, -5.0};
vec3_t cubeRotation = {0.0, 0.0, 0.0};

traingle_t trianglesToRender[MESH_FACES_COUNT];



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

vec2_t project(vec3_t point) {
    vec2_t projectedPoint = {
        (fovFactor * point.x) / point.z,
        (fovFactor * point.y) / point.z
    };
    return projectedPoint;
}

void update(void) {
    for (uint32_t i = 0U; i < MESH_FACES_COUNT; ++i) {
        face_t currentFace = meshFaces[i];
        vec3_t faceVertices[3] = {};

        faceVertices[0] = meshVertices[currentFace.a - 1U];
        faceVertices[1] = meshVertices[currentFace.b - 1U];
        faceVertices[2] = meshVertices[currentFace.c - 1U];

        traingle_t projectedTriangle;
        
        cubeRotation.x += 0.001F;
        cubeRotation.y += 0.001F;
        cubeRotation.z += 0.001F;

        for (uint32_t j = 0U; j < 3U; ++j) {
            vec3_t currentVertex = faceVertices[j];
            currentVertex = vec3RotateX(currentVertex, cubeRotation.x);
            currentVertex = vec3RotateY(currentVertex, cubeRotation.y);
            currentVertex = vec3RotateZ(currentVertex, cubeRotation.z);
            currentVertex.z -= cameraPosition.z;
            vec2_t projectedVertex = project(currentVertex);
            projectedVertex.x += winWidth / 2;
            projectedVertex.y += winHeight / 2;
            projectedTriangle.points[j] = projectedVertex;
        }
        trianglesToRender[i] = projectedTriangle;
    }
}

void render(void)
{
    SDL_RenderClear(renderer);
    clearColorBuffer(0x00);
    update();

    for (uint32_t i = 0U; i < MESH_FACES_COUNT; ++i) {
        traingle_t currentTriangle = trianglesToRender[i];
        drawFillRectangle(currentTriangle.points[0].x, currentTriangle.points[0].y, 5, 5, 0xFFFFFF00);
        drawFillRectangle(currentTriangle.points[1].x, currentTriangle.points[1].y, 5, 5, 0xFFFFFF00);
        drawFillRectangle(currentTriangle.points[2].x, currentTriangle.points[2].y, 5, 5, 0xFFFFFF00);
    }

    renderColorBuffer();
    SDL_RenderPresent(renderer);
}

int main(void)
{
    setup();

    while(isRunning) {
        SDL_Delay(FPS);
        processInput();
        render();
        renderColorBuffer();
    }

    destroyWindow();
    return 0;
}