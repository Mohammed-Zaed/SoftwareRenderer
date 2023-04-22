#ifndef _MESH_H_
#define _MESH_H_

#include <stdint.h>
#include "vector.h"
#include "triangle.h"

#define CUBE_VERTICES_COUNT  8U
#define CUBE_FACES_COUNT 12U

extern vec3_t cubeVertices[CUBE_VERTICES_COUNT];
extern face_t cubeFaces[CUBE_FACES_COUNT];

typedef struct {
    vec3_t* vertices;
    face_t* faces;
    vec3_t  rotation;
} mesh_t;

extern mesh_t mesh;

void loadCubeMeshData(void);
void loadObjData(const char* file);
#endif