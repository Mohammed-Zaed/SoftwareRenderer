#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdio.h>
#include "vector.h"

typedef struct {
    float m[4][4];
} mat4_t;

mat4_t mat4MakeIdentity(void);
mat4_t mat4MakeScale(const float sx, const float sy, const float sz);
mat4_t mat4MakeTranslate(const float tx, const float ty, const float tz);
vec4_t mat4Mulvec4(const mat4_t m, const vec4_t v);
mat4_t mat4MakeRotX(const float angle);
mat4_t mat4MakeRotY(const float angle);
mat4_t mat4MakeRotZ(const float angle);
mat4_t mat4MulMat4(const mat4_t a, const mat4_t b);
mat4_t mat4MakePerspective(const float fov, const float aspect, const float zNear, const float zFar);
vec4_t mat4MulProjectionVec4(mat4_t projection, vec4_t v);
mat4_t mat4LookAt(vec3_t eye, vec3_t target, vec3_t up);
void mat4Print(mat4_t mat);

#endif