#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mesh.h"
#include "array.h"
#include "sys/stat.h"

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
    {1U, 2U, 3U, 0xFFFF0000},
    {1U, 3U, 4U, 0xFFFF0000},
    //right
    {4U, 3U, 5U, 0xFF00FF00},
    {4U, 5U, 6U, 0xFF00FF00},
    //back
    {6U, 5U, 7U, 0xFF0000FF},
    {6U, 7U, 8U, 0xFF0000FF},
    //left
    {8U, 7U, 2U, 0xFFFFFF00},
    {8U, 2U, 1U, 0xFFFFFF00},
    //top
    {2U, 7U, 5U, 0xFF00FFFF},
    {2U, 5U, 3U, 0xFF00FFFF},
    //bottom
    {6U, 8U, 1U, 0xFFCDCDCD},
    {6U, 1U, 4U, 0xFFCDCDCD}
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

void loadObjData(const char* filePath) {

    FILE* file = fopen(filePath, "r");

    if (file == NULL) {
        fprintf(stderr, "Error: File open failed.\n");
    } else {
        char line[1024];
        while(fgets(line, 1024, file)) {
            if (strncmp(line, "v ", 2) == 0) {
                vec3_t vertex;
                sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
                array_push(mesh.vertices, vertex);
            }

            if (strncmp(line, "f ", 2) == 0) {
                int32_t vertexIndices[3];
                int32_t textureIndices[3];
                int32_t normalIndices[3];
                sscanf(
                    line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &vertexIndices[0], &textureIndices[0], &normalIndices[0],
                    &vertexIndices[1], &textureIndices[1], &normalIndices[1],
                    &vertexIndices[2], &textureIndices[2], &normalIndices[2]
                    );

                face_t face = {vertexIndices[0], vertexIndices[1], vertexIndices[2]};
                array_push(mesh.faces, face);
            }
        }
        fclose(file);
    }


}