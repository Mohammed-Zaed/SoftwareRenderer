#ifndef _MESH_H_
#define _MESH_H_

#include <stdint.h>
#include "vector.h"
#include "triangle.h"

#define MESH_VERTICES_COUNT  8U
#define MESH_FACES_COUNT 12U

extern vec3_t meshVertices[MESH_VERTICES_COUNT];
extern face_t meshFaces[MESH_FACES_COUNT];

#endif