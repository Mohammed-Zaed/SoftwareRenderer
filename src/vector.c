#include <math.h>
#include "vector.h"


vec3_t vec3RotateX(vec3_t v, float angle) {
    vec3_t rotatedX = {
        v.x,
        v.y * cos(angle) - v.z * sin(angle),
        v.y * sin(angle) + v.z * cos(angle),
    };
    return rotatedX;
}

vec3_t vec3RotateY(vec3_t v, float angle) {
    vec3_t rotatedY = {
        v.x * cos(angle) - v.z * sin(angle),
        v.y,
        v.x * sin(angle) + v.z * cos(angle),
    };
    return rotatedY;
}

vec3_t vec3RotateZ(vec3_t v, float angle) {
    vec3_t rotatedZ = {
        v.x * cos(angle) - v.y * sin(angle),
        v.x * sin(angle) + v.y * cos(angle),
        v.z
    };
    return rotatedZ;
}