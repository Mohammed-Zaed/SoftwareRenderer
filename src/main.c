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
#include "camera.h"
#include "clipping.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* colorBufferTexture = NULL;

uint32_t winWidth = 800;
uint32_t winHeight = 600;

uint32_t* colorBuffer = NULL;
float* zBuffer = NULL;
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

light_t globalLightSource = {0.00, 000.00, 1.00};
const uint32_t MAX_TRIANGLES_PER_MESH = 10000U;
traingle_t trianglesToRender[MAX_TRIANGLES_PER_MESH];
uint32_t trianglesToRenderForCurrentFrame;
mat4_t projectionMatrix;
uint32_t shadedColor = 0xFFFFFFFF;
float deltaTime;
// uint32_t shadedColor = 0xFFCDCDCD;

void setup(void)
{
    isRunning = !initWindow();
    
    colorBuffer = (uint32_t*)malloc(sizeof(uint32_t) * winWidth * winHeight);
    zBuffer = (float*)malloc(sizeof(float) * winWidth * winHeight);
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
    
    const float aspecty = (float)winHeight / (float)winWidth;
    const float aspectx = (float)winWidth / (float)winHeight;
    const float fovy = M_PI / 3.0F;
    const float fovx = atan(tan(fovy / 2) * aspectx) * 2.0F;
    const float zNear = 0.1F;
    const float zFar = 100.0F;
    projectionMatrix = mat4MakePerspective(fovy, aspecty, zNear, zFar);
    initFrustumPlanes(fovx, fovy, zNear, zFar);

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
                
                case SDLK_x:
                    // Disable back face culling
                    isBackFaceCulling = false;
                break;

                case SDLK_UP:
                    camera.position.y += 3.0 * deltaTime;
                break;
                
                case SDLK_DOWN:
                    camera.position.y -= 3.0 * deltaTime;
                break;
                
                case SDLK_a:
                    camera.yaw -= 1.0 * deltaTime;
                break;
                
                case SDLK_d:
                    camera.yaw += 1.0 * deltaTime;
                break;

                case SDLK_w:
                    camera.forwardVelocity = vec3Mul(camera.direction, 5.0 * deltaTime);
                    camera.position = vec3Add(camera.position, camera.forwardVelocity);
                break;

                case SDLK_s:
                    camera.forwardVelocity = vec3Mul(camera.direction, 5.0 * deltaTime);
                    camera.position = vec3Sub(camera.position, camera.forwardVelocity);
                break;
                }
    }
}

void update(void) {
    uint32_t timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - prevFrameTime);

    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }
    
    deltaTime = (SDL_GetTicks() - prevFrameTime) / 1000.00F;
    prevFrameTime = SDL_GetTicks();

    static float sx = 1.00F;
    static float sy = 1.00F;
    static float sz = 1.00F;

    static float tx = 0.0F;
    static float ty = 0.0F;
    static float tz = 4.0F;
    
    const vec3_t up = {0, 1, 0};

    // Initialize the target looking at the positive z-axis
    vec3_t target = { 0, 0, 1 };
    mat4_t cameraYawRotation = mat4MakeRotY(camera.yaw);
    camera.direction = vec4ToVec3(mat4Mulvec4(cameraYawRotation, vec3ToVec4(target)));

    // Offset the camera position in the direction where the camera is pointing at
    target = vec3Add(camera.position, camera.direction);
    vec3_t up_direction = { 0, 1, 0 };

    const mat4_t viewMatrix = mat4LookAt(camera.position, target, up);
    trianglesToRenderForCurrentFrame = 0U;
    uint32_t meshFacesCount = array_length(mesh.faces);
    for (uint32_t i = 0U; i < meshFacesCount; ++i) {
        face_t currentFace = mesh.faces[i];
        vec3_t faceVertices[3];

        faceVertices[0] = mesh.vertices[currentFace.a];
        faceVertices[1] = mesh.vertices[currentFace.b];
        faceVertices[2] = mesh.vertices[currentFace.c];
                   
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
            vec4CurrentVertex = mat4Mulvec4(viewMatrix, vec4CurrentVertex);
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
        vec3_t origin = {0.0F, 0.0F, 0.0F};
        vec3_t cameraRay = vec3Normalise(vec3Sub(origin, a));
        float isFrontFacing = vec3Dot(normal, cameraRay);
        bool cullFace = isFrontFacing <= 0.0F ? true : false;
        // End of Backface culling algorithm

        // Culling is Enabled when back face culling is on. 
        bool renderFace = !cullFace || !isBackFaceCulling;
        
        if (!renderFace) {
            continue;
        }
        

        polygon_t polygon = createPolygon(
                                transformedVertices[0], transformedVertices[1], transformedVertices[2], 
                                currentFace.uva, currentFace.uvb, currentFace.uvc
                                );
        int32_t beforeClippingVerticesCount = polygon.numVertices;
        clipPolygon(&polygon);
        traingle_t trianglesAfterClipping[MAX_NO_POLY_TRIANGLES];
        tex2_t texCoordsAfterClipping[MAX_NO_POLY_TRIANGLES];
        int32_t numOfTrianglesAFterClipping = 0;
        triangleFromPolygon(&polygon, trianglesAfterClipping,  &numOfTrianglesAFterClipping);
        
        // if (true) {
        for (uint32_t k = 0U; k < numOfTrianglesAFterClipping; ++k) {
            
            traingle_t currentTriangle = trianglesAfterClipping[k];

            vec4_t projectedPoints[3];
            if (numOfTrianglesAFterClipping > 0) {
                for (uint32_t j = 0U; j < 3U; ++j)
                {
                    vec3_t currentVertex = vec4ToVec3(currentTriangle.points[j]);
                    vec4_t tempVertex = mat4MulProjectionVec4(projectionMatrix, vec3ToVec4(currentVertex));
                    tempVertex.y *= -1.0F;
                    tempVertex.x *= winWidth / 2;
                    tempVertex.y *= winHeight / 2;
                    tempVertex.x += winWidth / 2;
                    tempVertex.y += winHeight / 2;
                    projectedPoints[j] = tempVertex;
                }
                tex2_t texCoord[3] = {
                    currentTriangle.texCoord[0],
                    currentTriangle.texCoord[1],
                    currentTriangle.texCoord[2],
                };
                
                traingle_t projectedTriangle = {
                    .points = {projectedPoints[0], projectedPoints[1], projectedPoints[2]},
                    .texCoord = {texCoord[0], texCoord[1], texCoord[2]},
                    .color = currentFace.color,
                };
                if (trianglesToRenderForCurrentFrame < MAX_TRIANGLES_PER_MESH) {
                    trianglesToRender[trianglesToRenderForCurrentFrame++] = projectedTriangle;
                }
            }
        }
    }
}

void render(void)
{
    SDL_RenderClear(renderer);
    clearColorBuffer(0x00);
    update();

    const int32_t numOfTriangles = trianglesToRenderForCurrentFrame;
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
                currentTriangle.points[0].x, currentTriangle.points[0].y, currentTriangle.points[0].z, currentTriangle.points[0].w,
                currentTriangle.points[1].x, currentTriangle.points[1].y, currentTriangle.points[1].z, currentTriangle.points[1].w,
                currentTriangle.points[2].x, currentTriangle.points[2].y, currentTriangle.points[2].z, currentTriangle.points[2].w,
                currentTriangle.color 
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
    trianglesToRenderForCurrentFrame = 0;
    renderColorBuffer();
    SDL_RenderPresent(renderer);
    clearZBuffer();
}

void freeResources(void) {
    array_free(mesh.vertices);
    array_free(mesh.faces);
    free(colorBuffer);
    free(zBuffer);
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