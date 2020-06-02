#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera{
    private:
    point3 origin;
    point3 lowerLeftCorner;
    point3 horizontalDir;
    point3 verticalDir;

    public:
        camera(){
            const auto aspectRatio = 16.0 / 9.0;
            auto viewportHeight = 2.0;
            auto viewportWidth = viewportHeight * aspectRatio;
            auto focalLength = 1.0;

            origin = point3(0,0,0);
            horizontalDir = vec3(viewportWidth, 0, 0);
            verticalDir = vec3(0, viewportHeight, 0);
            lowerLeftCorner = origin - horizontalDir/2 - verticalDir/2 - vec3(0, 0, focalLength);
    }
    ray getRay(double u, double v){
        return ray(origin, lowerLeftCorner + u*horizontalDir + v*verticalDir - origin);
    }
};

#endif