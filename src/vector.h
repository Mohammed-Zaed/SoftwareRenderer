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

//TODO::Vector Operations.
vec3_t vec3RotateX(vec3_t v, float angle);
vec3_t vec3RotateY(vec3_t v, float angle);
vec3_t vec3RotateZ(vec3_t v, float angle);


#endif