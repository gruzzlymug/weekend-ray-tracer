#include "hl.h"
#include "ray.h"
#include "s2.h"
#include <iostream>
#include <limits>
#include <random>

std::ostream& operator<<(std::ostream& os, const vec3& v) {
    os << "[" << v.x() << ", " << v.y() << ", " << v.z() << "]";
    return os;
}


class camera {
    vec3 hz;
    vec3 vt;
    vec3 cn;
    vec3 og;

    public:
        camera();
        ray get_ray(float u, float v);
};

camera::camera()
    : hz(4.0, 0.0, 0.0),
      vt(0.0, 2.0, 0.0),
      cn(-2.0, -1.0, -1.0),
      og(0.0, 0.0, 0.0)
{
    // empty
}

ray camera::get_ray(float u, float v) {
    ray r(og, cn + u*hz + v*vt - og);
    return r;
}


vec3 rand_in_unit_sphere() {
    vec3 p;
    do {
        p = 2 * vec3(drand48(), drand48(), drand48()) - vec3{1, 1, 1};
    } while (dot(p, p) > 1.0);
    return p;
}

vec3 reflect(const vec3& incident, const vec3& normal) {
    vec3 r = incident - 2*dot(incident, normal)*normal;
    return r;
}

class material {
    public:
        virtual bool scatter(const ray& incident, const hit_wreck& rec, vec3& attenuation, ray& scattered) = 0;
};

class phong : public material {
    vec3 albedo;

    public:
        phong(const vec3& a)
            : albedo{a}
        {
            // empty
        }
        bool scatter(const ray& incident, const hit_wreck& rec, vec3& attenuation, ray& scattered) {
            vec3 light{-2,1,4};
            vec3 to_light = light - rec.p;
            vec3 to_eye = vec3(0,0,0)-rec.p;
            vec3 h = unit_vector(to_eye - to_light);
            vec3 rv = rec.normal + rand_in_unit_sphere();
            float n_dot_h = dot(h, rec.normal);
            float n = n_dot_h*n_dot_h;
            float nn = n*n*n*n*n*n*n*n*n*n;
            float nnn = nn*nn*nn*nn*nn*nn;
            attenuation = n * albedo + nnn * vec3(1,1,1);
            vec3 xyz = unit_vector(rv - rec.p);
            //scattered = ray(rec.p, rv - rec.p);
            scattered = ray(rec.p, xyz);
            return true;
        }
};

class matte : public material {
    vec3 albedo;

    public:
        matte(const vec3& a)
            : albedo{a}
        {
            // empty
        }
        bool scatter(const ray& incident, const hit_wreck& rec, vec3& attenuation, ray& scattered) {
            vec3 rv = rec.normal + rand_in_unit_sphere();
            attenuation = albedo;
            scattered = ray(rec.p, rv - rec.p);
            return true;
        }
};

class metal : public material {
    vec3 albedo;

    public:
        metal(const vec3& a)
            : albedo{a}
        {

        }
        bool scatter(const ray& incident, const hit_wreck& rec, vec3& attenuation, ray& scattered) {
            vec3 r = reflect(incident.direction(), rec.normal);
            r += 0.4*rand_in_unit_sphere();
            r = unit_vector(r);
            if (dot(rec.normal, r) > 0) {
                attenuation = albedo;
                scattered = ray(rec.p, r);
                return true;
            }
            return false;
        }
};

vec3 color(const ray& r, const scene& hitables, int depth=0) {
    hit_wreck rec;
    float tmax = std::numeric_limits<float>::max();
    bool hit_something = hitables.hit(r, 0.001, tmax, rec);
    if (hit_something) {
        auto m = rec.mat.lock();
        if (m) {
            vec3 attenuation;
            ray scat;
            if (depth < 50 && m->scatter(r, rec, attenuation, scat)) {
                return attenuation * color(scat, hitables, depth+1);
            } else {
                // absorbed!
                return vec3(0,0,0);
            }
        }
    }
    vec3 unit_dir = unit_vector(r.direction());
    float t = 0.5*(unit_dir.y()+1.0);
    return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5,0.7,1.0);
}


int main() {
    std::shared_ptr<material> mtl = std::make_shared<phong>(vec3(0.8,0,0));
    std::shared_ptr<material> mtl2 = std::make_shared<matte>(vec3(0.4,0.5,0.4));
    std::shared_ptr<material> mtl3 = std::make_shared<metal>(vec3(0.8,0.8,0.8));

    vec3 cc(-0.0, 0.0, -2.00);
    float rr = 1.0;
    s2 s{cc, rr, mtl};
    vec3 dd(0.0, -100.0, -2.00);
    s2 q{dd, 99.0, mtl2};
    vec3 ee(-2.0, 0.0, -2.00);
    s2 w{ee, rr, mtl3};

    std::vector<s2> things;
    things.emplace_back(s);
    things.emplace_back(q);
    things.emplace_back(w);
    scene hitables{things};

    camera cam;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0, 1);

    int nx = 400;
    int ny = 200;
    int ns = 100;

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            vec3 clr{0, 0, 0};
            for (int s = 0; s < ns; ++s) {
                float u = float(i + dist(mt)) / float(nx);
                float v = float(j + dist(mt)) / float(ny);
                ray r = cam.get_ray(u, v);
                clr += color(r, hitables);
            }
            clr /= ns;
            // gamma correct (p259)
            clr = vec3(sqrt(clr[0]), sqrt(clr[1]), sqrt(clr[2]));
            std::cout << int(255.99 * clr[0]) << " ";
            std::cout << int(255.99 * clr[1]) << " ";
            std::cout << int(255.99 * clr[2]) << " ";
            std::cout << "\n";
        }
    }

    return 0;
}
