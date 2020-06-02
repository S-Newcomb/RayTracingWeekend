#ifndef VEC3_H
#define VEC3_H

#include "rtweekend.h"

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3{
    public:
        vec3() : e{0,0,0} {};
        vec3(double e0, double e1, double e2) : e{e0, e1, e2}{}

        double x() const { return e[0];}
        double y() const { return e[1];}
        double z() const { return e[2];}

        vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
        double operator[] (int i) const {return e[i]; }
        double& operator[] (int i) {return e[i]; }

        vec3& operator+= (const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }
        
        vec3& operator *= (const double t){
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        vec3& operator/= (const double t){
            return *this *= 1/t;
        }

        double length() const{
            return sqrt(length_squared());
        }

        double length_squared() const {
            return e[0] *e[0] + e[1] *e[1] + e[2] *e[2];
        }

        public: 
            double e[3];
        inline static vec3 random(){
            return vec3(randomDouble(), randomDouble(), randomDouble());
        }   

        inline static vec3 random(double min, double max){
            return vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
        }


};

//Type aliases for vec3
using point3 = vec3; //3D Point
using color = vec3; // RGB Color

//vec3 utility functions
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
     return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(v.e[0] + u.e[0], v.e[1] + u.e[1], v.e[2] + u.e[2]);
} 

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(v.e[0] * u.e[0], v.e[1] * u.e[1], v.e[2] * u.e[2]);
} 

inline vec3 operator*(const double t, const vec3 &v) {
    return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
} 

inline vec3 operator*(vec3 &v, const double t) {
    return t * v;
}

inline vec3 operator/(vec3 &v, const double t) {
    return (1/t) * v;
} 

inline double dot(const vec3& u, const vec3& v){
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 &u, const vec3& v){
    return vec3(u.e[1] * v.e[2] - v.e[1] * u.e[2],
                u.e[0] * v.e[2] - v.e[0] * u.e[2],
                u.e[1] * v.e[0] - v.e[1] * u.e[0]);
}

inline vec3 unit_vector(vec3& v){
    return v / v.length();
}

vec3 randomInUnitSphere(){
    while (true){
        auto p = vec3::random(-1, 1);
        if(p.length_squared() >= 1) continue;
            return p;
    }
}

vec3 randomUnitVector(){
    auto a = randomDouble(0, 2*pi);
    auto z = randomDouble(-1, 1);
    auto r = sqrt(1 - z*z);
    return vec3(r*cos(a), r*sin(a), z);
}

vec3 randomInHemisphere(const vec3& normal){
    vec3 inUnitSphere = randomInUnitSphere();
    if  (dot(inUnitSphere, normal) > 0,0) //In the same hemisphere as the normal
        return inUnitSphere;
    else
        return -inUnitSphere;
}

vec3 reflect(const vec3& v, const vec3& n){
    return v - 2*dot(v,n)*n;
}

#endif
