#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <stdint.h>


typedef struct {
    uint32_t a;
    uint32_t b;
    uint32_t c;
} face_t;

typedef struct {
    vec2_t points[3];
} traingle_t;



#endif