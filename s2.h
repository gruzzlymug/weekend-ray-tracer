#ifndef _S2_H_
#define _S2_H_

#include "hw.h"
#include "ray.h"
#include "vec3.h"

class s2
{
    public:
        vec3 center;
        float radius;

        s2() :
            center{0,0,0},
            radius{1.0}
        { /* empty */ }
        s2(const vec3& c, float r) :
            center{c},
            radius{r}
        { /* empty */ }

        bool hit(const ray& r, float tmin, float tmax, hit_wreck& rec) const;
};


// derivation of formula
//   dot((r.A + t*r.B - s.center), (r.A + t*r.B - s.center)) << "\n";
//   dot((m + t*r.B), (m + t*r.B)) << "\n";
//   dot(m, m) + 2*t*dot(m,r.B) + t*t*dot(r.B, r.B) << "\n";
// conversion to quadratic parts ax^2 + bx + c = 0, using radius now...
// factor out redundant 2s
// t1 is on the surface facing the camera, t2 is facing away (probably)
// TODO is t2 necessary? (dot(normal, camera) instead...?)
bool s2::hit(const ray& r, float tmin, float tmax, hit_wreck& rec) const {
    vec3 m{r.A - center};
    float a = dot(r.B, r.B);
    float b = dot(m, r.B);
    float c = dot(m, m) - (radius * radius);
    // note if r.B is normalized, a = 1 and can be removed
    float discriminant = b*b - a*c;
    if (discriminant > 0.0) {
        // find hit point
        float t1 = (-b - sqrt(discriminant)) / a;
        if (t1 > tmin && t1 < tmax) {
            rec.t = t1;
            rec.p = r.point_at_parameter(t1);
            rec.normal = unit_vector(rec.p - center); // or use: / radius;
            return true;
        }
        // check t2
        float t2 = (-b + sqrt(discriminant)) / a;
        if (t2 > tmin && t2 < tmax) {
            rec.t = t2;
            rec.p = r.point_at_parameter(t2);
            rec.normal = unit_vector(rec.p - center);
            return true;
        }
    }
    return false;
}

#endif

