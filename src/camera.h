#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector.h"

typedef struct {
    vec3_t position;
    vec3_t direction;
    vec3_t forwardVelocity;
    float  yaw;
} camera_t;

extern camera_t camera;


#endif