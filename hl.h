#ifndef _SCENE_H_
#define _SCENE_H_

#include "s2.h"
#include "ray.h"
#include <vector>
#include <iostream>

class scene {
    private:
        std::vector<s2> _things;

    public:
        scene() {}
        scene(const std::vector<s2> things)
            : _things{things}
        { /* empty */ }

        bool hit(const ray& r, float tmin, float tmax, hit_wreck& rec) const;
};

bool scene::hit(const ray& r, float tmin, float tmax, hit_wreck& rec) const {
    bool hit_anything = false;

    hit_wreck temp_rec;
    float closest_t = tmax;
    for (auto i : _things) {
        bool hit_something = i.hit(r, tmin, closest_t, temp_rec);
        if (hit_something) {
            hit_anything = true;
            closest_t = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#endif

