#include <math.h>
#include "matrix.h"

mat4_t mat4MakeIdentity(void) {

    // |1.00F 0.00F 0.00F 0.00F|
    // |0.00F 1.00F 0.00F 0.00F|
    // |0.00F 0.00F 1.00F 0.00F|
    // |0.00F 0.00F 0.00F 1.00F|
    
    const mat4_t result = {
        1.00F, 0.00F, 0.00F, 0.00F,
        0.00F, 1.00F, 0.00F, 0.00F,
        0.00F, 0.00F, 1.00F, 0.00F,
        0.00F, 0.00F, 0.00F, 1.00F
    };

    return result;
};

mat4_t mat4MakeScale(const float sx, const float sy, const float sz) {

    // |sx    0.00F 0.00F 0.00F|
    // |0.00F sy    0.00F 0.00F|
    // |0.00F 0.00F sz    0.00F|
    // |0.00F 0.00F 0.00F 1.00F|
    
    mat4_t result = mat4MakeIdentity();
    result.m[0][0] = sx;
    result.m[1][1] = sy;
    result.m[2][2] = sz;
    

    return result;
};

mat4_t mat4MakeTranslate(const float tx, const float ty, const float tz) {
    
    // |1.00F 0.00F 0.00F tx   |
    // |0.00F 1.00F 0.00F ty   |
    // |0.00F 0.00F 1.00F tz   |
    // |0.00F 0.00F 0.00F 1.00F|
    
    mat4_t result = mat4MakeIdentity();
    result.m[0][3] = tx;
    result.m[1][3] = ty;
    result.m[2][3] = tz;

    return result;
}

vec4_t mat4Mulvec4(const mat4_t m, const vec4_t v) {
    const vec4_t result = {
        v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3],
        v.x * m.m[1][0] + v.y * m.m[1][1] + v.z * m.m[1][2] + v.w * m.m[1][3],
        v.x * m.m[2][0] + v.y * m.m[2][1] + v.z * m.m[2][2] + v.w * m.m[2][3],
        v.x * m.m[3][0] + v.y * m.m[3][1] + v.z * m.m[3][2] + v.w * m.m[3][3],
    };
    return result;
}

mat4_t mat4MakeRotX(const float angle) {
    const float c = cos(angle);
    const float s = sin(angle);
    mat4_t result = mat4MakeIdentity();

    result.m[1][1] = c;
    result.m[1][2] = -s;
    result.m[2][1] = s;
    result.m[2][2] = c;

    return result;
}

mat4_t mat4MakeRotY(const float angle) {
    const float c = cos(angle);
    const float s = sin(angle);
    mat4_t result = mat4MakeIdentity();

    result.m[0][0] = c;
    result.m[0][1] = s;
    result.m[3][1] = -s;
    result.m[3][2] = c;

    return result;
}

mat4_t mat4MakeRotZ(const float angle) {
    const float c = cos(angle);
    const float s = sin(angle);
    mat4_t result = mat4MakeIdentity();

    result.m[0][0] = c;
    result.m[0][1] = -s;
    result.m[1][0] = s;
    result.m[1][1] = c;

    return result;
}