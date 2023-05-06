#include <math.h>
#include <stdio.h>
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

float vec2Length(const vec2_t v) {
    const float magnitude = sqrtf((v.x * v.x) + (v.y * v.y));
    return magnitude;
}

vec2_t vec2Add(const vec2_t v1, const vec2_t v2) {
    const vec2_t result = {v1.x + v2.x, v1.y + v2.y};
    return result;
}

vec2_t vec2Sub(const vec2_t v1, const vec2_t v2) {
    const vec2_t result = {v1.x - v2.x, v1.y - v2.y};
    return result;
}

vec2_t vec2Mul(const vec2_t v, const float f) {
    const vec2_t result = {f * v.x, f * v.y};
    return result;
}

vec2_t vec2Div(const vec2_t v, const float f) {
    const vec2_t result = {v.x / f, v.y / f};
    return result;
}

float vec2Dot(const vec2_t v1, const vec2_t v2) {
    const float result = (v1.x * v2.x) + (v1.y * v2.y);
    return result;
}

vec2_t vec2Normalise(const vec2_t v) {
    const vec2_t result = vec2Div(v, vec2Length(v));
    return result;
}

float vec3Length(const vec3_t v) {
    const float magnitude = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
    return magnitude;
}

vec3_t vec3Add(const vec3_t v1, const vec3_t v2) {
    const vec3_t result = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
    return result;
}

vec3_t vec3Sub(const vec3_t v1, const vec3_t v2) {
    const vec3_t result = {v1.x - v2.x, v1.y - v2.y, v1.z - v2.z};
    return result;
}

vec3_t vec3Mul(const vec3_t v, const float f) {
    const vec3_t result = {f * v.x, f * v.y, f * v.z};
    return result;
}

vec3_t vec3Div(const vec3_t v, const float f) {
    const vec3_t result = {v.x / f, v.y / f, v.z / f};
    return result;
}

vec3_t vec3Cross(const vec3_t v1, const vec3_t v2) {
    const vec3_t result = {
           //x = aybz - azby
           .x = v1.y * v2.z - v1.z * v2.y,
           //y = azbx - axbz
           .y = v1.z * v2.x - v1.x * v2.z,
           //z = axby - aybx 
           .z = v1.x * v2.y - v1.y * v2.x
    };
    return result;
}

float vec3Dot(const vec3_t v1, const vec3_t v2) {
    const float result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    return result;
}

vec3_t vec3Normalise(const vec3_t v) {
    const vec3_t result = vec3Div(v, vec3Length(v));
    return result;
}

vec4_t vec3ToVec4(const vec3_t v) {
    const vec4_t result = {
        v.x, v.y, v.z, 1.0F
    };
    return result;
}

vec2_t vec4ToVec2(const vec4_t v) {
    const vec2_t result = {
        v.x, v.y
    };
    return result;
}

vec3_t vec4ToVec3(const vec4_t v) {
    const vec3_t result = {
        v.x, v.y, v.z
    };
    return result;
}

void vec3Print(vec3_t v) {
    printf("(%f,\t%f,\t%f)\n", v.x, v.y, v.z);
}

vec3_t vec3Create(const float x, const float y, const float z) {
    const vec3_t result = {x, y, z};
    return result;
}