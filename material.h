#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
    public:
        virtual bool scatter(
            const ray& in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

double schlick(double cosine, double ref_idx){
    auto r0 = (1- ref_idx) / (1 + ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0) * pow((1 - cosine), 5);
}

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

class dialectric : public material {
    public:
    double ref_idx;
    dialectric(double ri) : ref_idx(ri){}

    virtual bool scatter(
        const ray& in, const hit_record& rec, color& attenuation, ray& scattered
    ) const {
        attenuation = color(1.0, 1.0, 1.0);
        double etai_over_etat = (rec.frontFace) ? (1.0 / ref_idx) : ref_idx;
    
        auto inDirection = in.direction();
        vec3 unitDirection = unit_vector(inDirection);
        double cosTheta = fmin(dot(-unitDirection, rec.normal), 1.0);
        double sinTheta = sqrt(1.0 - cosTheta*cosTheta);
        if (etai_over_etat * sinTheta > 1.0){
            //Must reflect
            vec3 reflected = reflect(unitDirection, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }
        //Can reflect
        double reflectProb = schlick(cosTheta, etai_over_etat);
        if (randomDouble() < reflectProb){
            vec3 reflected = reflect(unitDirection, rec.normal);
            scattered = ray(rec.p, reflected);
            return true;
        }

        vec3 refracted = refract(unitDirection, rec.normal, etai_over_etat);
        scattered = ray(rec.p, refracted);
        return true;
    }
};



#endif