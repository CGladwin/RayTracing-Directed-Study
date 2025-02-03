
#include "include/hittable.hpp"
#include "include/hittable_list.hpp"
#include "include/sphere.hpp"
#include "include/camera.hpp"

int main(int argc, char* argv[]) {
    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    auto aspect_ratio = 16.0 / 9.0;
    auto image_width  = 400;
    auto samples_per_pixel = 100;
    auto max_depth = 50;

    camera cam = camera(aspect_ratio,image_width,samples_per_pixel,max_depth);
    cam.render(world,argc,argv);
        
}