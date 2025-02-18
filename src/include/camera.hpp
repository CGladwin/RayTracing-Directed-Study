#pragma once
#include <chrono> 
#include "common.hpp"
#include "base_classes.hpp"
#include "hittable.hpp"
#include "my_stb_image.h"
#include "my_exception.hpp"
#include "material.hpp"

class camera {
private:
    int    image_height;   // Rendered image height
    point3 camera_center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    double pixel_samples_scale;  // Color scale factor for a sum of pixel samples
    std::vector<u_int8_t> pixels = {};


    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pixel_samples_scale = 1.0 / samples_per_pixel;

        camera_center = point3(0, 0, 0);

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }   

    /**
     * @brief returns rgb for pixel color value
     * 
     * if sphere is not hit, color is based on a a predefined gradient
     * 
     * if sphere is hit, color is based on lambertian material
     * 
     * @param r 
     * @param world 
     * @return color 
    */
    color ray_color(const ray& r, int depth, const hittable& world) const {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0,0,0);
        
        hit_record rec;

        if (world.hit(r, interval(0.001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                return attenuation * ray_color(scattered, depth-1, world);
            return color(0,0,0);
        }
        /**
         * lerp used to create background gradient
         * blendedValue=(1−a)⋅startValue+a⋅endValue
         * here, startValue = white, endValue = blue
         * 
         * if something was hit, it will affect r.direction vec3, which will tint the color
         * (since the loss of energy on hits will result in smaller vectors)
         * 
         */

        vec3 unit_direction = unit_vector(r.direction());
        auto background_gradient = 0.5*(unit_direction.y() + 1.0);
        // 
        return (1.0-background_gradient)*color(1.0, 1.0, 1.0) + background_gradient*color(0.5, 0.7, 1.0);
    }

    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = camera_center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

public:
    double aspect_ratio;  // Ratio of image width over height
    int    image_width;  // Rendered image width in pixel count
    int    samples_per_pixel;   // Count of random samples for each pixel
    int    max_depth;   // Maximum number of ray bounces into scene

    // Constructor
    camera(double aspect_ratio = 1.0,int image_width = 100,int samples_per_pixel = 10, int max_depth = 10) 
            : aspect_ratio(aspect_ratio), image_width(image_width), samples_per_pixel(samples_per_pixel), max_depth(max_depth)
    {
        initialize();
        pixels.reserve(image_width * image_height * 3); // Reserve space for RGB pixels
    }

    void generate_png(int argc, char* argv[]){
        const char* output_path;
        if (argc < 2) {
            output_path = "src/images/output.png";
        }
        else {
            output_path = argv[1];
        }
        if (!stbi_write_png(output_path, image_width, image_height, /* RGB */ 3, pixels.data(), image_width * 3)) {
            throw_line("failed to write PNG file");
        }
        cout<<"Success! Result image located at "<<output_path<<"\n";

    }

    void render(const hittable& world,int argc, char* argv[]) {
        auto start_time = std::chrono::high_resolution_clock::now(); 
        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(pixels, pixel_samples_scale * pixel_color);
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> duration = end_time - start_time; 
        std::clog << "\rDone raytracing! Program completed in "<<duration.count()<< " seconds. \nWriting to file...\n";
        generate_png(argc,argv);
    }

};