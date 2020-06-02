#ifndef SPHERE_H
#define SPERE_H

#include "hittable.h"

class sphere : public hittable{
    public:
        sphere(){}
        sphere(point3 cen, double r, shared_ptr<material> m) 
        : center(cen), radius(r), mat_ptr(m){};
        
        point3 center;
        double radius;
        shared_ptr<material> mat_ptr;

        virtual bool hit(const ray& r, double tmin, double tmax, hit_record& rec) const;
};

bool sphere::hit(const ray& r, double tmin, double tmax, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto half_b = dot(oc, r.direction());
    auto c = dot(oc, oc) - (radius*radius);
    auto discriminant =  half_b * half_b - a*c;

    if (discriminant > 0){
        //Check if closest intersection is in range
        auto root = sqrt(discriminant);
        auto temp = (-half_b - root)/a;
        if (temp > tmin && temp < tmax){
            rec.t = temp;
            rec.p = r.at(rec.t);
            auto  pointMinusCenter = (rec.p - center);
            vec3 outwardNormal =  pointMinusCenter / radius;
            rec.setFaceNormal(r, outwardNormal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
        //Otherwise check if second intersection is in range
        temp = (-half_b + root) / a;
        if (temp > tmin && temp < tmax){
            rec.t = temp;
            rec.p = r.at(rec.t);
            auto  pointMinusCenter = (rec.p - center);
            vec3 outwardNormal =  pointMinusCenter / radius;
            rec.setFaceNormal(r, outwardNormal);
            rec.mat_ptr = mat_ptr;

            return true;
        }
    }
    return false;
}

#endif