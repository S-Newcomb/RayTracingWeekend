#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

// class camera{
//     private:
//     point3 origin;
//     point3 lowerLeftCorner;
//     point3 horizontalDir;
//     point3 verticalDir;

//     public:
//         camera(
//             point3 lookFrom,
//             point3 lookAt,
//             vec3 vup,
//             double vfov, //vertical field of view in degrees
//             double aspectRatio
//             ){
//             auto theta = degrees_to_radians(vfov);
//             auto h = tan(theta/2);
//             auto viewportHeight = 2.0 * h;
//             auto viewportWidth = viewportHeight * aspectRatio;

//             auto w = unit_vector(lookFrom - lookAt);
//             auto u = unit_vector(cross(vup, w));
//             auto v = cross(w, u);

//             origin = lookFrom;
//             horizontalDir = viewportWidth * u;
//             verticalDir = viewportHeight * v;
//             lowerLeftCorner = origin - horizontalDir/2 - verticalDir/2 - w;
//     }
//     ray getRay(double u, double v) const{
//         return ray(origin, lowerLeftCorner + u*horizontalDir + v*verticalDir - origin);
//     }
// };

class camera {
    public:
        camera(
            point3 lookfrom,
            point3 lookat,
            vec3   vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio,
            double aperture,
            double focusDist
        ) {
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2.0 * h;
            auto viewport_width = aspect_ratio * viewport_height;

            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal = focusDist * viewport_width * u;
            vertical = focusDist * viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focusDist*w;

            lensRadius = aperture / 2;
        }

        ray getRay(double s, double t) const {
            vec3 rd = lensRadius * randomInUnitDisk();
            vec3 offset = u * rd.x() + v * rd.y();
            return ray(
                origin + offset, 
                lower_left_corner + s*horizontal + t*vertical - origin - offset
                );
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 u, v, w;
        double lensRadius;
};


#endif