from point_and_vec import *
from ray_class import *
import os
from PPM_image_output import *
import cProfile

def hit_sphere(sphere_center: point3, radius: float, ray: ray) -> float:
    # vector from center of sphere to camera, used to declare where the sphere exists in virtual 3d space
    # origin_to_center.dot(origin_to_center) == (Cx - x)^2 + (Cy - y)^2 + (Cz - z)^2 = r^2 
    # In the above C is a point in space
    oc: vec3 = sphere_center - ray.origin_point
    # coefficients in the equation at^2 + bt + c = 0, where t is the scalar multiple of the ray's direction in the equation P(t) = Q + td
    a = (ray.direction_vec).dot(ray.direction_vec) 
    b = -2.0 * (ray.direction_vec).dot(oc)
    c = oc.dot(oc) - radius**2
    # find the number of solutions
    discriminant = b**2 - 4*a*c
    if (discriminant < 0):
        # no solutions, ray doesn't hit sphere
        return -1.0
    else:
        # return the smallest solution for t, which is the closes hit point to the ray origin
        return (-b - math.sqrt(discriminant)) / (2.0*a)

def ray_colour(r: ray) -> color:
    # define the sphere and check if it hit the ray in the param
    t = hit_sphere(point3(0,0,-1), 0.5, r)
    # the sphere is in front of the camera for now
    # so we aren't worrying about negative t values
    if (t > 0.0):
        # r.at(t) = point on surface of sphere, facing camera
        # vec3(0,0,-1) is a vector from the origin to the sphere's center
        # N = the unit vector pointing normal to the surface of the sphere at point t
        # unit vector values are between -1 and 1
        N: vec3 = (r.at(t) - vec3(0,0,-1)).unit_vector()
        # shift the values so that they're 0, 2, then 0 , 1
        # equivalent to color.from_vec3_like((N + vec3(1,1,1)) /2)
        return color(N.x+1,N.y+1,N.z+1) / 2
    unit_direction = (r.direction_vec).unit_vector()
    a = 0.5*(unit_direction.y + 1.0)
    # Lerping between (255, 255, 255) which is white to a light shade blue (128, 255*0.7, 255)
    return color(1.0, 1.0, 1.0)*(1.0-a) + color(0.5, 0.7, 1.0)*a
    
def main():
    
    aspect_ratio = 16.0 / 9.0 #ideal ratio
    image_width = 400

    # Calculate the image height, and ensure that it's at least 1.
    image_height = int(image_width / aspect_ratio)
    if image_height < 1: 
        image_height = 1
    viewport_height = 2.0
    integer_ratio = image_width/image_height
    viewport_width = viewport_height * integer_ratio

    #Camera
    focal_length = 1.0
    viewport_height = 2.0
    viewport_width = viewport_height * integer_ratio
    camera_center = point3(0, 0, 0) #camera center at the origin

    # Calculate the vectors across the horizontal and down the vertical viewport edges.
    viewport_u = vec3(viewport_width, 0, 0)
    viewport_v = vec3(0, -viewport_height, 0)

    # Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u: vec3 = viewport_u / image_width
    pixel_delta_v: vec3 = viewport_v / image_height

    # Calculate the location of the upper left pixel.
    viewport_upper_left: vec3 = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2
    # starts with camera center, moves in z axis to intersect with viewport, moves left and up
    pixel00_loc: vec3 = viewport_upper_left + (pixel_delta_u + pixel_delta_v)/2
    # find the top-left aka first pixel


    # Render
    output_ppm_dir: str = "images"
    output_ppm_path = os.path.abspath(output_ppm_dir)+'\\first_camera_image.ppm'

    with open(output_ppm_path,"w+") as f:
        f.write("P3\n%d %d\n255\n" % (image_width,image_height))
        for j in range(image_height):
            progress_indicator("scanlines remaining: ",image_height,image_height-j)
            for i in range(image_width):
                # find each pixel in viewport by offsetting 0,0 based on pixel deltas
                pixel_center = pixel00_loc + (pixel_delta_u * i) + (pixel_delta_v * j)
                ray_direction = pixel_center - camera_center
                r = ray(camera_center,ray_direction)
                pixel_colour = ray_colour(r)
                f.write(pixel_colour.write_colour())
    print("\nDone.") #newline necessary because output stream after progress_indicator call is pointing to end of previous line, not newline
    view_ppm_img(output_ppm_path)

if __name__ == "__main__":
    # for profiling code
    # cProfile.run('main()',sort='tottime')
    main()
            