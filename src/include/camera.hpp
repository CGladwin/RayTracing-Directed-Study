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
    point3 camera_center;  // origin of rays being cast
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    double pixel_samples_scale;  // Color scale factor for a sum of pixel samples
    std::vector<u_int8_t> pixels = {};
    vec3   u, v, w;              // Camera frame basis vectors
    vec3   defocus_disk_u;       // Defocus disk horizontal radius
    vec3   defocus_disk_v;       // Defocus disk vertical radius


    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        pixel_samples_scale = 1.0 / samples_per_pixel;

        camera_center = lookfrom;

        // Determine viewport dimensions.
        // auto focal_length = (lookfrom - lookat).length();

        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = camera_center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }   

    /**
     * @brief Compute the color contribution along a ray path using recursive ray tracing.
     * 
     * This function traces a ray through the scene, accumulating color contributions from light emission
     * and surface scattering. It recursively evaluates up to a specified depth to simulate global illumination.
     * 
     * @param r The ray being traced
     * @param depth Remaining recursion depth (stops at 0 to prevent infinite loops)
     * @param world The hittable objects in the scene
     * @return The total computed color along the ray's path
     */
    color ray_color(const ray& r, int depth, const hittable& world) const {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0,0,0);
        
        hit_record rec;

        // If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        
        // Get color contribution from light emission at hit point
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);
    
        // If material doesn't scatter light, return only emission color
        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;
    
        // Recursively compute color contribution from scattered ray, 
        // attenuated by material's scattering properties
        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);
    
        // Combine emission and scattering contributions
        return color_from_emission + color_from_scatter;
    }

    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? camera_center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

public:
    double aspect_ratio;  // Ratio of image width over height
    int    image_width;  // Rendered image width in pixel count
    int    samples_per_pixel;   // Count of random samples for each pixel
    int    max_depth;   // Maximum number of ray bounces into scene

    point3 lookfrom = point3(0,0,0);   // Point camera is looking from
    point3 lookat   = point3(0,0,-1);  // Point camera is looking at
    vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction
    double vfov = 90;  // Vertical view angle (field of view)

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus
    color  background;               // Scene background color

    // Constructor
    camera(double aspect_ratio = 1.0,int image_width = 100,int samples_per_pixel = 10, int max_depth = 10) 
            : aspect_ratio(aspect_ratio), image_width(image_width), samples_per_pixel(samples_per_pixel), max_depth(max_depth)
    {
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
        initialize();
        pixels.reserve(image_width * image_height * 3); // Reserve space for RGB pixels

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