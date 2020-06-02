

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

hittable_list randomScene() {
    hittable_list world;
    auto grountMaterial = make_shared<lambertian>(color(0.5,0.5,0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, grountMaterial));
    for (int a = -11; a < 11; a++){
        for (int b = -11; b < 11; b++){
            auto chooseMat = randomDouble();
            point3 center = point3(a + 0.9*randomDouble(), 0.2, b + 0.9 *randomDouble());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9){
                 shared_ptr<material> sphereMaterial;
                
                  if (chooseMat < 0.8){
                      //Diffuse
                    auto albedo = color::random() * color::random();
                    sphereMaterial = make_shared<lambertian>(albedo);
                      world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                  }
                   else if (chooseMat < 0.95){
                       //Metal
                    auto albedo = color::random(0.5, 1);
                    double fuzz = randomDouble(0, 0.5);
                    sphereMaterial = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));
                }
                else{
                    //Glass
                    sphereMaterial = make_shared<dialectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphereMaterial));                       
                }
            }
        }
    }
    auto material1 = make_shared<dialectric>(1.5);
    world.add(make_shared<sphere>(point3(0,1,0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4,0.2,0.1));
    world.add(make_shared<sphere>(point3(-4,1,0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7,0.6,0.5), 0.0);
    world.add(make_shared<sphere>(point3(4,1,0), 1.0, material3));

    return world;
}

int main(){
    const auto aspectRatio = 16.0 / 9.0;
    const int imageWidth = 384;
    const int imageHeight = static_cast<int>(imageWidth/aspectRatio);
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    auto R = cos(pi/4);
    //Add spheres to world
    hittable_list world = randomScene();

    point3 lookfrom(13,2,3);
    point3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

camera cam(lookfrom, lookat, vup, 20, aspectRatio, aperture, dist_to_focus);
   // camera cam(90, double(imageWidth)/imageHeight);
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
