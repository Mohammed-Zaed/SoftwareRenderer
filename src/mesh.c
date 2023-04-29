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
    { .a = 1, .b = 2, .c = 3, .uva = { 0, 0 }, .uvb = { 0, 1 }, .uvc = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 1, .b = 3, .c = 4, .uva = { 0, 0 }, .uvb = { 1, 1 }, .uvc = { 1, 0 }, .color = 0xFFFFFFFF },
    // right
    { .a = 4, .b = 3, .c = 5, .uva = { 0, 0 }, .uvb = { 0, 1 }, .uvc = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 4, .b = 5, .c = 6, .uva = { 0, 0 }, .uvb = { 1, 1 }, .uvc = { 1, 0 }, .color = 0xFFFFFFFF },
    // back
    { .a = 6, .b = 5, .c = 7, .uva = { 0, 0 }, .uvb = { 0, 1 }, .uvc = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 6, .b = 7, .c = 8, .uva = { 0, 0 }, .uvb = { 1, 1 }, .uvc = { 1, 0 }, .color = 0xFFFFFFFF },
    // left
    { .a = 8, .b = 7, .c = 2, .uva = { 0, 0 }, .uvb = { 0, 1 }, .uvc = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 8, .b = 2, .c = 1, .uva = { 0, 0 }, .uvb = { 1, 1 }, .uvc = { 1, 0 }, .color = 0xFFFFFFFF },
    // top
    { .a = 2, .b = 7, .c = 5, .uva = { 0, 0 }, .uvb = { 0, 1 }, .uvc = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 2, .b = 5, .c = 3, .uva = { 0, 0 }, .uvb = { 1, 1 }, .uvc = { 1, 0 }, .color = 0xFFFFFFFF },
    // bottom
    { .a = 6, .b = 8, .c = 1, .uva = { 0, 0 }, .uvb = { 0, 1 }, .uvc = { 1, 1 }, .color = 0xFFFFFFFF },
    { .a = 6, .b = 1, .c = 4, .uva = { 0, 0 }, .uvb = { 1, 1 }, .uvc = { 1, 0 }, .color = 0xFFFFFFFF }
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