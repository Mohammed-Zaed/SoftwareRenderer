#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#include "display.h"
#include "array.h"
#include "vector.h"
#include "light.h"
#include "matrix.h"
#include "texture.h"
#include "triangle.h"
#include "mesh.h"
#include "upng.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* colorBufferTexture = NULL;

uint32_t winWidth = 800;
uint32_t winHeight = 600;

uint32_t* colorBuffer = NULL;
static bool isRunning = false;
static bool isWireFrame = true;
static bool isVertices = true;
static bool isTriangle = false;
static bool isTexturedTriangle = true;
static bool isBackFaceCulling = true;

const uint32_t FPS = 33;
const uint32_t fovFactor = 800.00F;
const uint32_t FRAME_TARGET_TIME = 1000 / 33;

uint32_t prevFrameTime = 0;

vec3_t cameraPosition = {0.0, 0.0, 0.0};
light_t globalLightSource = {0.00, 000.00, 1.00};
traingle_t* trianglesToRender = NULL;
mat4_t projectionMatrix;
uint32_t shadedColor = 0xFFFFFFFF;
// uint32_t shadedColor = 0xFFCDCDCD;

void setup(void)
{
    isRunning = !initWindow();
    
    colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * winWidth * winHeight);
    if (!colorBuffer) {
        fprintf(stderr, "Error:: Memory allocation for color buffer failed.\n");
    }

    colorBufferTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        winWidth,
        winHeight
    );
    
    const float fov = M_PI / 3.0F;
    const float aspect = (float)winHeight / (float)winWidth;
    const float zNear = 0.1F;
    const float zFar = 100.0F;
    projectionMatrix = mat4MakePerspective(fov, aspect, zNear, zFar);

    // loadCubeMeshData();
    loadObjData("./assets/cube.obj");
    loadPngTextureData("./assets/cube.png");
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
            switch(event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    isRunning = false;
                break;
                
                case SDLK_1:
                    // Display wire fram and red dot
                    isWireFrame = true;
                    isVertices = true;
                    isTriangle = false;
                    isTexturedTriangle = false;
                break;

                case SDLK_2:
                    // Display only the wireframe lines.
                    isWireFrame = true;
                    isVertices = false;
                    isTriangle = false;
                    isTexturedTriangle = false;
                break;

                case SDLK_3:
                    // Display filled triangles with solid color.
                    isWireFrame = false;
                    isVertices = false;
                    isTriangle = true;
                    isTexturedTriangle = false;
                break;

                case SDLK_4:
                    //TODO:: Display both triangles and wireframe lines.
                    isWireFrame = true;
                    isVertices = false;
                    isTriangle = true;
                    isTexturedTriangle = false;
                break;

                case SDLK_5:
                    // Display both only vertices. 
                    isWireFrame = false;
                    isVertices = true;
                    isTriangle = false;
                    isTexturedTriangle = false;
                break;

                case SDLK_6:
                    // Display both triangles and wireframe lines and vertices.
                    isWireFrame = true;
                    isVertices = true;
                    isTriangle = true;
                    isTexturedTriangle = false;
                break;
                
                case SDLK_7:
                    isWireFrame = true;
                    isVertices = true;
                    isTriangle = false;
                    isTexturedTriangle = true;
                break;

                case SDLK_8:
                    isWireFrame = false;
                    isVertices = false;
                    isTriangle = false;
                    isTexturedTriangle = true;
                break;

                case SDLK_c:
                    // Enable back face culling
                    isBackFaceCulling = true;
                break;
                
                case SDLK_d:
                    // Disable back face culling
                    isBackFaceCulling = false;
                break;
            }
    }
}

void update(void) {
    uint32_t timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - prevFrameTime);

    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }
    static float sx = 1.00F;
    static float sy = 1.00F;
    static float sz = 1.00F;

    static float tx = 0.0F;
    static float ty = 0.0F;
    static float tz = 0.0F;

    uint32_t meshFacesCount = array_length(mesh.faces);
    for (uint32_t i = 0U; i < meshFacesCount; ++i) {
        face_t currentFace = mesh.faces[i];
        vec3_t faceVertices[3];

        faceVertices[0] = mesh.vertices[currentFace.a];
        faceVertices[1] = mesh.vertices[currentFace.b];
        faceVertices[2] = mesh.vertices[currentFace.c];

        // mesh.rotation.x += 0.0001F;
        mesh.rotation.y += 0.0001F;
        // mesh.rotation.z += 0.0001F;
        
        // if (sx >= 1.50F) {
        //     sx = 1.00F;
        // } else {
        //     sx += 0.000001F;
        // }

        // if (tx >= 0.50F) {
        //     tx = 0.00F;
        // } else {
        //     tx += 0.0001F;
        // }

        // sy = sx;
        // ty = tx;
                   
        vec3_t transformedVertices[3];
        for (uint32_t j = 0U; j < 3U; ++j) {
            vec3_t currentVertex = faceVertices[j];

            vec4_t vec4CurrentVertex = vec3ToVec4(currentVertex);
            mat4_t scalerMatrix = mat4MakeScale(sx, sy, sz); 
            mat4_t matRotX = mat4MakeRotX(mesh.rotation.x);
            mat4_t matRotY = mat4MakeRotY(mesh.rotation.y);
            mat4_t matRotZ = mat4MakeRotZ(mesh.rotation.z);
            mat4_t translationMatrix = mat4MakeTranslate(tx, ty, tz);

            mat4_t worldMatrix = mat4MakeIdentity();
            worldMatrix = mat4MulMat4(scalerMatrix, worldMatrix);
            worldMatrix = mat4MulMat4(worldMatrix, matRotX);
            worldMatrix = mat4MulMat4(worldMatrix, matRotY);
            worldMatrix = mat4MulMat4(worldMatrix, matRotZ);
            worldMatrix = mat4MulMat4(translationMatrix, worldMatrix);
            vec4CurrentVertex = mat4Mulvec4(worldMatrix, vec4CurrentVertex);
            vec4CurrentVertex.z += 5.0F;
            
            transformedVertices[j] = vec4ToVec3(vec4CurrentVertex);
        }

        {
        // Start of flat shading
        // Three verticies of transformed triangle face
        const vec3_t a = transformedVertices[0];
        const vec3_t b = transformedVertices[1];
        const vec3_t c = transformedVertices[2];

        vec3_t ab = vec3Sub(b, a);
        vec3_t ac = vec3Sub(c, a);
        vec3_t normal = vec3Normalise(vec3Cross(ab, ac));
        vec3_t lightRay = vec3Normalise(vec3Sub(globalLightSource.direction, a));
        float illuminationFactor = vec3Dot(normal, lightRay);
        currentFace.color = lightApplyIntensity(shadedColor,  illuminationFactor);
        // End of flat shading algorithm
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
        bool cullFace = isFrontFacing <= 0.0F ? true : false;
        // End of Backface culling algorithm

        // Culling is Enabled when back face culling is on. 
        bool renderFace = !cullFace || !isBackFaceCulling;
        
        vec4_t projectedPoints[3];

        if (renderFace) {
            for (uint32_t j = 0U; j < 3U; ++j)
            {
                vec3_t currentVertex = transformedVertices[j];
                vec4_t tempVertex = mat4MulProjectionVec4(projectionMatrix, vec3ToVec4(currentVertex));
                tempVertex.y *= -1.0F;
                tempVertex.x *= winWidth / 2;
                tempVertex.y *= winHeight / 2;
                tempVertex.x += winWidth / 2;
                tempVertex.y += winHeight / 2;
                projectedPoints[j] = tempVertex;
            }
            tex2_t texCoord[3] = {
                {currentFace.uva.u, currentFace.uva.v},
                {currentFace.uvb.u, currentFace.uvb.v},
                {currentFace.uvc.u, currentFace.uvc.v}
            };
            
            traingle_t projectedTriangle = {
                .points = {projectedPoints[0], projectedPoints[1], projectedPoints[2]},
                .texCoord = {texCoord[0], texCoord[1], texCoord[2]},
                .color = currentFace.color,
                .avgDepth = (transformedVertices[0].z + transformedVertices[1].z + transformedVertices[2].z) / 3.00F
            };

            array_push(trianglesToRender, projectedTriangle);
        }
    }
    // Sort the triangles to render by their avgDepth
    uint32_t numTriangles = array_length(trianglesToRender);
    for (uint32_t i = 0U; i < numTriangles; ++i) {
        for (uint32_t j = i; j < numTriangles; ++j) {
            if (trianglesToRender[i].avgDepth < trianglesToRender[j].avgDepth) {
                // Swap the triangles positions in the array
                traingle_t temp = trianglesToRender[i];
                trianglesToRender[i] = trianglesToRender[j];
                trianglesToRender[j] = temp;
            }
        }
    }
}

void render(void)
{
    SDL_RenderClear(renderer);
    clearColorBuffer(0x00);
    update();

#if 1
    const int32_t numOfTriangles = array_length(trianglesToRender);
    for (uint32_t i = 0U; i < numOfTriangles; ++i) {
        traingle_t currentTriangle = trianglesToRender[i];
        if (isWireFrame) {
            drawTriangle(
                currentTriangle.points[0].x, currentTriangle.points[0].y,
                currentTriangle.points[1].x, currentTriangle.points[1].y,
                currentTriangle.points[2].x, currentTriangle.points[2].y,
                0xFFFFFF00
            );
        }

        if (isTriangle) {
            drawFilledTriangle(
                currentTriangle.points[0].x, currentTriangle.points[0].y,
                currentTriangle.points[1].x, currentTriangle.points[1].y,
                currentTriangle.points[2].x, currentTriangle.points[2].y,
                currentTriangle.color//0xFFDCDCDC //currentTriangle.color
                );
        }

        if (isVertices) {
            drawFillRectangle(currentTriangle.points[0].x, currentTriangle.points[0].y, 3U, 3U, 0xFFFF0000);
            drawFillRectangle(currentTriangle.points[1].x, currentTriangle.points[1].y, 3U, 3U, 0xFFFF0000);
            drawFillRectangle(currentTriangle.points[2].x, currentTriangle.points[2].y, 3U, 3U, 0xFFFF0000);
        }
        
        if (isTexturedTriangle) {
            drawTexturedTriangle(
                currentTriangle.points[0].x, currentTriangle.points[0].y, currentTriangle.points[0].z, currentTriangle.points[0].w, currentTriangle.texCoord[0].u, currentTriangle.texCoord[0].v,
                currentTriangle.points[1].x, currentTriangle.points[1].y, currentTriangle.points[1].z, currentTriangle.points[1].w, currentTriangle.texCoord[1].u, currentTriangle.texCoord[1].v,
                currentTriangle.points[2].x, currentTriangle.points[2].y, currentTriangle.points[2].z, currentTriangle.points[2].w, currentTriangle.texCoord[2].u, currentTriangle.texCoord[2].v,
                meshTexture
                );
        }
    }
    array_free(trianglesToRender);
    trianglesToRender = NULL;
#else
    drawTriangle(300, 100, 50,400, 500, 700, 0xFF00FF00);
    drawFilledTriangle(300, 100, 50,400, 500, 700, 0xFF0000FF);
#endif
    renderColorBuffer();
    SDL_RenderPresent(renderer);
}

void freeResources(void) {
    array_free(mesh.vertices);
    array_free(mesh.faces);
    free(colorBuffer);
    upng_free(pngTexture);
    meshTexture = NULL;
}

int main(void)
{
    setup();

    while(isRunning) {
        processInput();
        render();
        renderColorBuffer();
    }
    freeResources();
    destroyWindow();
    return 0;
}