#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "rtweekend.h"
class material;

struct hit_record{
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool frontFace;

    inline void setFaceNormal(const ray& r , const vec3& outward_normal){
            //if the dot product of the direction and normal is less than 0, the ray is outside the sphere
            frontFace = dot(r.direction(), outward_normal) < 0;
            normal = frontFace ? outward_normal : -outward_normal;
    }
};

class hittable{
    public:
        virtual bool hit (const ray&r, double t_min, double t_max, hit_record& rec) const = 0;
};


#endif