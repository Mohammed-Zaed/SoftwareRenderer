#ifndef _CLIPPING_H_
#define _CLIPPING_H_

#include "vector.h"

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

void initFrustumPlanes(float fov, float zNear, float zFar);

#endif