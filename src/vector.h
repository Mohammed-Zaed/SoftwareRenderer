#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct {
    float x;
    float y;
}vec2_t;

typedef struct {
    float x;
    float y;
    float z;
}vec3_t;

typedef struct {
    float x;
    float y;
    float z;
    float w;
}vec4_t;


//TODO::Vector Operations.
float vec2Length(const vec2_t v);
vec2_t vec2Add(const vec2_t v1, const vec2_t v2);
vec2_t vec2Sub(const vec2_t v1, const vec2_t v2);
vec2_t vec2Mul(const vec2_t v, const float f);
vec2_t vec2Div(const vec2_t v, const float f);
float vec2Dot(const vec2_t v1, const vec2_t v2);
vec2_t vec2Normalise(const vec2_t v);

float vec3Length(const vec3_t v);
vec3_t vec3Add(const vec3_t v1, const vec3_t v2);
vec3_t vec3Sub(const vec3_t v1, const vec3_t v2);
vec3_t vec3Mul(const vec3_t v, const float f);
vec3_t vec3Div(const vec3_t v, const float f);
vec3_t vec3Cross(const vec3_t v1, const vec3_t v2);
float vec3Dot(const vec3_t v1, const vec3_t v2);

vec3_t vec3RotateX(vec3_t v, float angle);
vec3_t vec3RotateY(vec3_t v, float angle);
vec3_t vec3RotateZ(vec3_t v, float angle);
vec3_t vec3Normalise(const vec3_t v);

vec4_t vec3ToVec4(const vec3_t v);
vec3_t vec4ToVec3(const vec4_t v);


#endif