#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
    public:
        virtual bool scatter(
            const ray& in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material {
    public:
        color albedo;
        lambertian(const color& a) : albedo(a){}

        virtual bool scatter(
            const ray& in, const hit_record& rec, color& attenuation, ray& scattered
        ) const {
            vec3 scatteredDirection = rec.normal + randomUnitVector();
            scattered = ray(rec.p, scatteredDirection);
            attenuation = albedo;
            return true;
        }

};

class metal : public material{
    public:
        color albedo;
        double fuzz;

        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(
            const ray& in, const hit_record& rec, color& attenuation, ray& scattered
        ) const {
            vec3 inDirection = in.direction();
            vec3 reflected = reflect(unit_vector(inDirection), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * randomInUnitSphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
};

#endif