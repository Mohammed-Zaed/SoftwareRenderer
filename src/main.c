#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#include "display.h"
#include "array.h"
#include "vector.h"
#include "triangle.h"
#include "mesh.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* colorBufferTexture = NULL;

uint32_t winWidth = 800;
uint32_t winHeight = 600;

uint32_t* colorBuffer = NULL;
static bool isRunning = false;

const uint32_t FPS = 33;
const uint32_t fovFactor = 800.00F;
const uint32_t FRAME_TARGET_TIME = 1000 / 33;

uint32_t prevFrameTime = 0;

vec3_t cameraPosition = {0.0, 0.0, 0.0};
traingle_t* trianglesToRender = NULL;

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
    
    loadObjData("./assets/f22.obj");
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
    uint32_t timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - prevFrameTime);

    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    uint32_t meshFacesCount = array_length(mesh.faces);
    for (uint32_t i = 0U; i < meshFacesCount; ++i) {
        face_t currentFace = mesh.faces[i];
        vec3_t faceVertices[3];

        faceVertices[0] = mesh.vertices[currentFace.a - 1U];
        faceVertices[1] = mesh.vertices[currentFace.b - 1U];
        faceVertices[2] = mesh.vertices[currentFace.c - 1U];

        traingle_t projectedTriangle;
        
        mesh.rotation.x += 0.0001F;
        // mesh.rotation.y += 0.001F;
        // mesh.rotation.z += 0.001F;

        vec3_t transformedVertices[3];
        for (uint32_t j = 0U; j < 3U; ++j) {
            vec3_t currentVertex = faceVertices[j];
            currentVertex = vec3RotateX(currentVertex, mesh.rotation.x);
            currentVertex = vec3RotateY(currentVertex, mesh.rotation.y);
            currentVertex = vec3RotateZ(currentVertex, mesh.rotation.z);
            currentVertex.z += 5.0F;
            transformedVertices[j] = currentVertex;
        }
        
        // Start of Back face culling for algorithm LHS
        // Three verticies of transformed triangle face
        const vec3_t a = transformedVertices[0];
        const vec3_t b = transformedVertices[1];
        const vec3_t c = transformedVertices[2];
        
        vec3_t ab = vec3Sub(b, a);
        vec3_t ac = vec3Sub(c, a);
        vec3_t normal = vec3Normalise(vec3Cross(ab, ac));
        vec3_t cameraRay = vec3Normalise(vec3Sub(cameraPosition, a));
        float isFrontFacing = vec3Dot(normal, cameraRay);
        // End of Backface culling algorithm

        if (isFrontFacing > 0.0F) {
            for (uint32_t j = 0U; j < 3U; ++j)
            {
                vec3_t currentVertex = transformedVertices[j];
                vec2_t projectedVertex = project(currentVertex);
                projectedVertex.x += winWidth / 2;
                projectedVertex.y += winHeight / 2;
                projectedTriangle.points[j] = projectedVertex;
            }
            array_push(trianglesToRender, projectedTriangle);
        }
    }
}

void render(void)
{
    SDL_RenderClear(renderer);
    clearColorBuffer(0x00);
    update();

    const int32_t numOfTriangles = array_length(trianglesToRender);
    for (uint32_t i = 0U; i < numOfTriangles; ++i) {
        traingle_t currentTriangle = trianglesToRender[i];

        drawTriangle(
            currentTriangle.points[0].x, currentTriangle.points[0].y, 
            currentTriangle.points[1].x, currentTriangle.points[1].y, 
            currentTriangle.points[2].x, currentTriangle.points[2].y, 
            0xFFFFFF00
        );
        
        drawFillRectangle(currentTriangle.points[0].x, currentTriangle.points[0].y, 3U, 3U, 0xFF0000FF);
        drawFillRectangle(currentTriangle.points[1].x, currentTriangle.points[1].y, 3U, 3U, 0xFF0000FF);
        drawFillRectangle(currentTriangle.points[2].x, currentTriangle.points[2].y, 3U, 3U, 0xFF0000FF);
    }
    array_free(trianglesToRender);
    trianglesToRender = NULL;

    renderColorBuffer();
    SDL_RenderPresent(renderer);
}

void freeResources(void) {
    array_free(mesh.vertices);
    array_free(mesh.faces);
    free(colorBuffer);
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
    freeResources();
    destroyWindow();
    return 0;
}