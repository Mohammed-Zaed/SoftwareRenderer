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


#endif