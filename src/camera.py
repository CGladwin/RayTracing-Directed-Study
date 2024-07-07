from point_and_vec import *
from ray_class import *
import os
from PPM_image_output import *

def ray_colour(r: ray) -> color:
    unit_direction = unit_vector(r.direction_vec)
    a = 0.5*(unit_direction.y() + 1.0)
    return (1.0-a)*color([1.0, 1.0, 1.0]) + a*color([0.5, 0.7, 1.0])

if __name__ == "__main__":
    
    aspect_ratio = 16.0 / 9.0
    image_width = 400

    # Calculate the image height, and ensure that it's at least 1.
    image_height = int(image_width / aspect_ratio)
    if image_height < 1: 
        image_height = 1
    viewport_height = 2.0
    viewport_width = viewport_height * (float(image_width)/image_height)

    #Camera

    focal_length = 1.0
    viewport_height = 2.0
    viewport_width = viewport_height * (float(image_width)/image_height)
    camera_center = point3([0, 0, 0])

    # Calculate the vectors across the horizontal and down the vertical viewport edges.
    viewport_u = vec3([viewport_width, 0, 0])
    viewport_v = vec3([0, -viewport_height, 0])

    # Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width
    pixel_delta_v = viewport_v / image_height

    # Calculate the location of the upper left pixel.
    viewport_upper_left = camera_center - vec3([0, 0, focal_length]) - viewport_u/2 - viewport_v/2
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v)

    # Render
    image_width : int =  256
    image_height : int = 256
    output_ppm_dir: str = "images"
    output_ppm_path = os.path.abspath(output_ppm_dir)+'\\image.ppm'

    with open(output_ppm_path,"w+") as f:
        f.write("P3\n%d %d\n255\n" % (image_width,image_height))
        for j in range(image_height):
            progress_indicator("scanlines remaining: ",image_height,image_height-j)
            for i in range(image_width):
                pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v)
                ray_direction = pixel_center - camera_center
                r = ray(camera_center,ray_direction)
                pixel_colour = ray_colour(r)
                f.write(pixel_colour.write_colour())
    print("\nDone.") #newline necessary because output stream after progress_indicator call is pointing to end of previous line, not newline
    view_ppm_img(output_ppm_path)
            

#     std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

#     for (int j = 0; j < image_height; j++) {
#         std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
#         for (int i = 0; i < image_width; i++) {
#             auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
#             auto ray_direction = pixel_center - camera_center;
#             ray r(camera_center, ray_direction);

#             color pixel_color = ray_color(r);
#             write_color(std::cout, pixel_color);
#         }
#     }

#     std::clog << "\rDone.                 \n";
# }