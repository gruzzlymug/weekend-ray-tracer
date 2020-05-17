#ifndef _HW_H_
#define _HW_H_

#include "vec3.h"

class material;

struct hit_wreck {
    float t;
    vec3 p;
    vec3 normal;
    std::weak_ptr<material> mat;
};

#endif

