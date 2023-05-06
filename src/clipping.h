#ifndef _CLIPPING_H_
#define _CLIPPING_H_

#include <stdint.h>
#include "vector.h"
#include "triangle.h"

#define MAX_NO_POLY_VERTICES  10U
#define MAX_NO_POLY_TRIANGLES  10U

enum {
    PLANE_LEFT_FRUSTUM = 0U,
    PLANE_RIGHT_FRUSTUM,
    PLANE_TOP_FRUSTUM,
    PLANE_BOTTOM_FRUSTUM,
    PLANE_FAR_FRUSTUM,
    PLANE_NEAR_FRUSTUM
};

typedef struct {
    vec3_t point;
    vec3_t normal;
} planes_t;

typedef struct {
    vec3_t vertices[MAX_NO_POLY_VERTICES];
    uint32_t numVertices;
} polygon_t;

void initFrustumPlanes(float fovx, float fovy, float zNear, float zFar);
polygon_t createPolygon(vec3_t v1, vec3_t v2, vec3_t v3);
void clipPolygon(polygon_t* polygon);
void clipPolygonAgainstPlane(polygon_t* polygon, int32_t plane);
void triangleFromPolygon(polygon_t* polygon, traingle_t* triangle, int32_t* numOfTrianglesGenerated);

#endif