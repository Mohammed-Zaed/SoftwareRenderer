#include <stdint.h>
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
    const vec4_t result = { v.x * m.m[0][0] + v.y * m.m[0][1] + v.z * m.m[0][2] + v.w * m.m[0][3],
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
    result.m[0][2] = s;
    result.m[2][0] = -s;
    result.m[2][2] = c;

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

mat4_t mat4MulMat4(const mat4_t a, const mat4_t b) {
    mat4_t result;
    for (uint32_t i = 0U; i < 4U; ++i) {
        for (uint32_t j = 0U; j < 4U; ++j) {
            result.m[i][j] = a.m[i][0] * b.m[0][j] + a.m[i][1] * b.m[1][j] + a.m[i][2] * b.m[2][j] + a.m[i][3] * b.m[3][j];
        }
    }
    return result;
}

mat4_t mat4MakePerspective(const float fov, const float aspect, const float zNear, const float zFar) {
    //(h/w) * 1/tan(fov/2)                    0                       0                   0
    //0                                       1/tan(fov/2)            0                   0
    //0                                       0                       zFar/(zFar-zNear)   (-zFar * zNear)/(zFar-zNear)
    //0                                       0                       1                   0
    mat4_t result = {{{0}}};
    result.m[0][0] = aspect * (1 / tan(fov/2));
    result.m[1][1] = (1 / tan(fov/2));
    result.m[2][2] = zFar / (zFar - zNear);
    result.m[2][3] = (-zFar * zNear) / (zFar - zNear);
    result.m[3][2] = 1;
    
    return result;
}

vec4_t mat4MulProjectionVec4(mat4_t projection, vec4_t v) {
    vec4_t result = mat4Mulvec4(projection, v);
    if (result.w != 0.0F) {
        result.x = result.x / result.w;
        result.y = result.y / result.w;
        result.z = result.z / result.w;
    }
    return result;
}