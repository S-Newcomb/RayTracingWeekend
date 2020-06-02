

#include "rtweekend.h"
#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <iostream>
#include <string>

color rayColor(const ray& r, const hittable& world, int depth){
    hit_record rec;
    //If we have exceeded the ray bounce limit no more light is gathered
    if (depth <= 0) return color(0,0,0);

    if (world.hit(r, 0.001, infinity, rec)){
        ray scattered;
        color attenuation;
        if (rec.mat_ptr -> scatter(r, rec, attenuation, scattered))
            return attenuation * rayColor(scattered, world, depth-1);
        return color(0,0,0);
    }
    vec3 direction = r.direction();
    vec3 unitDirection = unit_vector(direction);
    auto t = 0.5*(unitDirection.y() + 1.0);
    //color(1.0, 1.0, 1.0) = White
    //color(0.5, 0.7, 1.0) = Blue
    auto finalColor = (1.0-t) * color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
    return finalColor;
}

int main(){
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 384;
    const int imageHeight = static_cast<int>(imageWidth/aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    hittable_list world;
    //Add spheres to world
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5, make_shared<lambertian>(color(0.7, 0.3, 0.3))));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, make_shared<lambertian>(color(0.8, 0.8, 0.0))));
    world.add(make_shared<sphere>(point3(1,0,-1),0.5, make_shared<metal>(color(0.8,0.6,0.2))));
    world.add(make_shared<sphere>(point3(-1,0,-1),0.5, make_shared<metal>(color(0.8,0.8,0.8))));

    camera cam;

    for(int j = imageHeight - 1; j >= 0; j--){
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < imageWidth; i++){
            color pixelColor = color(0,0,0);
            for(int s = 0; s < samplesPerPixel; s++){
                auto u = (i + randomDouble()) / (imageWidth-1);
                auto v = (j + randomDouble()) / (imageHeight-1);
                ray r = cam.getRay(u, v);
                pixelColor += rayColor(r, world, maxDepth);
            }
            writeColor(std::cout, pixelColor, samplesPerPixel);
        }
    }
    std::cerr << "\nDone. \n";
}
