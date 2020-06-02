#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
    point3 orig;
    vec3 dir;

    //Creates empty array
    ray(){}
    
    //Creates array with origin and direction
    ray(const point3& origin, const vec3& direction)
    : orig(origin), dir(direction){}
    
    point3 origin() const {return orig;}
    point3 direction() const {return dir ;}

    point3 at(double t) const {
        return orig + t*dir;
    }
};

#endif
