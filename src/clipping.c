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

polygon_t createPolygon(vec3_t v0, vec3_t v1, vec3_t v2) {
    const polygon_t polygon = {
        .vertices = {v0, v1, v2},
        .numVertices = 3
        };
    
    return polygon;
}

void clipPolygon(polygon_t* polygon) {
    clipPolygonAgainstPlane(polygon, PLANE_LEFT_FRUSTUM);
    clipPolygonAgainstPlane(polygon, PLANE_RIGHT_FRUSTUM);
    clipPolygonAgainstPlane(polygon, PLANE_TOP_FRUSTUM);
    clipPolygonAgainstPlane(polygon, PLANE_BOTTOM_FRUSTUM);
    clipPolygonAgainstPlane(polygon, PLANE_NEAR_FRUSTUM);
    clipPolygonAgainstPlane(polygon, PLANE_FAR_FRUSTUM);
}

void clipPolygonAgainstPlane(polygon_t* polygon, int32_t plane) {
    vec3_t planePoint = planesFrustum[plane].point;
    vec3_t planeNormal = planesFrustum[plane].normal;
    
    vec3_t insideVerticesList[MAX_NO_POLY_VERTICES];
    vec3_t outsideVerticesList[MAX_NO_POLY_VERTICES];
    uint32_t countInsideVertices = 0U;
    uint32_t countOutsideVertices = 0U;
    
    vec3_t* currentVertex = &polygon->vertices[0];
    vec3_t* previousVertex = &polygon->vertices[polygon->numVertices - 1U];

    float previousDot = vec3Dot(vec3Sub(*previousVertex, planePoint), planeNormal);

    while (currentVertex != &polygon->vertices[polygon->numVertices]) {

        float currentDot = vec3Dot(vec3Sub(*currentVertex, planePoint), planeNormal);

        if (currentDot * previousDot < 0) {
            //TODO::Calculate interpolation factor t =  dotQ1 / (dotQ1 - dotQ2)
            const float t = previousDot / (previousDot - currentDot);
            //TODO::Calculate the intesection point I = Q1 + t(Q2 - Q1)
            const vec3_t intersectionPoint = vec3Add(*previousVertex, vec3Mul(vec3Sub(*currentVertex, *previousVertex), t)); 
            insideVerticesList[countInsideVertices] = intersectionPoint;
            countInsideVertices++;
        }
        
        if (currentDot > 0) {
            insideVerticesList[countInsideVertices] = *currentVertex;
            countInsideVertices++;
        }

        previousDot = currentDot;
        previousVertex = currentVertex;
        currentVertex++;
    }
    
    for (int32_t i = 0; i < countInsideVertices; ++i) {
        polygon->vertices[i] = insideVerticesList[i];
    }
    polygon->numVertices = countInsideVertices;

}
