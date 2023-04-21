#include "stdio.h"
#include "mesh.h"
#include "array.h"

mesh_t mesh = {
    NULL,
    NULL,
    {0.0F, 0.0F, 0.0F}
};

vec3_t cubeVertices[CUBE_VERTICES_COUNT] = {
    {-1.0F, -1.0F, -1.0F}, //1
    {-1.0F,  1.0F, -1.0F}, //2
    { 1.0F,  1.0F, -1.0F}, //3
    { 1.0F, -1.0F, -1.0F}, //4
    { 1.0F,  1.0F,  1.0F}, //5
    { 1.0F, -1.0F,  1.0F}, //6
    {-1.0F,  1.0F,  1.0F}, //7
    {-1.0F, -1.0F,  1.0F}  //8
};
face_t cubeFaces[CUBE_FACES_COUNT] = {
    //front
    {1U, 2U, 3U},
    {1U, 3U, 4U},
    //right
    {4U, 3U, 5U},
    {4U, 5U, 6U},
    //back
    {6U, 5U, 7U},
    {6U, 7U, 8U},
    //left
    {8U, 7U, 2U},
    {8U, 2U, 1U},
    //top
    {2U, 7U, 5U},
    {2U, 5U, 3U},
    //bottom
    {6U, 8U, 1U},
    {6U, 1U, 4U}
};

void loadCubeMeshData(void) {
    for (uint32_t i = 0U; i < CUBE_VERTICES_COUNT; ++i) {
        vec3_t cubeVertex = cubeVertices[i];
        array_push(mesh.vertices, cubeVertex);
    }

    for (uint32_t i = 0U; i < CUBE_FACES_COUNT; ++i) {
        face_t cubeFace = cubeFaces[i];
        array_push(mesh.faces, cubeFace);
    }
}