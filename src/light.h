#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <stdint.h>
#include "vector.h"

typedef struct {
    vec3_t direction;
} light_t;

uint32_t lightApplyIntensity(uint32_t color, float percentageFactor);

#endif