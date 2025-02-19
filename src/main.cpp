
#include "include/hittable.hpp"
#include "include/hittable_list.hpp"
#include "include/sphere.hpp"
#include "include/camera.hpp"
#include "include/material.hpp"

int main(int argc, char* argv[]) {
    hittable_list world;

    auto R = std::cos(pi/4);

    auto material_left  = make_shared<lambertian>(color(0,0,1));
    auto material_right = make_shared<lambertian>(color(1,0,0));

    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3( R, 0, -1), R, material_right));

    auto aspect_ratio = 16.0 / 9.0;
    auto image_width  = 400;
    auto samples_per_pixel = 100;
    auto max_depth = 50;

    camera cam = camera(aspect_ratio,image_width,samples_per_pixel,max_depth);
    cam.vfov = 90;
    cam.render(world,argc,argv);
        
}