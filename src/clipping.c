#include <math.h>
#include "clipping.h"

#define NUM_OF_FRUSTUM_PLANES 6U

planes_t planesFrustum[NUM_OF_FRUSTUM_PLANES];

void initFrustumPlanes(float fov, float zNear, float zFar)
{
    const float cosHalfFov = cos(fov / 2.0);
    const float sinHalFov = sin(fov / 2.0);
    
    vec3_t origin = {0, 0, 0};
    
    planesFrustum[PLANE_LEFT_FRUSTUM].point = vec3Create(0, 0, 0);
    planesFrustum[PLANE_LEFT_FRUSTUM].normal = vec3Create(cosHalfFov, 0, sinHalFov);
    
    planesFrustum[PLANE_RIGHT_FRUSTUM].point = vec3Create(0, 0, 0);
    planesFrustum[PLANE_RIGHT_FRUSTUM].normal = vec3Create(-cosHalfFov, 0, sinHalFov);

    planesFrustum[PLANE_TOP_FRUSTUM].point = vec3Create(0, 0, 0);
    planesFrustum[PLANE_TOP_FRUSTUM].normal = vec3Create(0, -cosHalfFov, sinHalFov);

    planesFrustum[PLANE_BOTTOM_FRUSTUM].point = vec3Create(0, 0, 0);
    planesFrustum[PLANE_BOTTOM_FRUSTUM].normal = vec3Create(0, cosHalfFov, sinHalFov);

    planesFrustum[PLANE_NEAR_FRUSTUM].point = vec3Create(0, 0, zNear);
    planesFrustum[PLANE_NEAR_FRUSTUM].normal = vec3Create(0, 0, 1);

    planesFrustum[PLANE_FAR_FRUSTUM].point = vec3Create(0, 0, zFar);
    planesFrustum[PLANE_FAR_FRUSTUM].normal = vec3Create(0, 0, -1);
}