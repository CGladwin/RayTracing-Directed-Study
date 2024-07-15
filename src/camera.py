from hittables import *
from point_and_vec import *
from ray_class import *
import os
from PPM_image_output import *
import cProfile

class  camera():
    def __init__(self, aspect_ratio: float, image_width: float) -> None:
        self.aspect_ratio = aspect_ratio
        self.image_width = image_width
    
    def initialize(self) -> None:
        # Calculate the image height, and ensure that it's at least 1.
        self.image_height = int(self.image_width / self.aspect_ratio)
        if self.image_height < 1: 
            self.image_height = 1
        viewport_height = 2.0
        integer_ratio = self.image_width/self.image_height
        viewport_width = viewport_height * integer_ratio

        #Camera
        focal_length = 1.0
        viewport_height = 2.0
        viewport_width = viewport_height * integer_ratio
        self.camera_center = point3(0, 0, 0) #camera center at the origin

        # Calculate the vectors across the horizontal and down the vertical viewport edges.
        viewport_u = vec3(viewport_width, 0, 0)
        viewport_v = vec3(0, -viewport_height, 0)

        # Calculate the horizontal and vertical delta vectors from pixel to pixel.
        self.pixel_delta_u: vec3 = viewport_u / self.image_width
        self.pixel_delta_v: vec3 = viewport_v / self.image_height

        # Calculate the location of the upper left pixel.
        viewport_upper_left: vec3 = self.camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2
        # starts with camera center, moves in z axis to intersect with viewport, moves left and up
        self.pixel00_loc: vec3 = viewport_upper_left + (self.pixel_delta_u + self.pixel_delta_v)/2
        # find the top-left aka first pixel

    def render(self,world: hittable,output_ppm_dir: str = "images"):
        self.initialize()
        output_ppm_path = os.path.abspath(output_ppm_dir)+'\\first_camera_image.ppm'
        with open(output_ppm_path,"w+") as f:
            f.write("P3\n%d %d\n255\n" % (self.image_width,self.image_height))
            for j in range(self.image_height):
                progress_indicator("scanlines remaining: ",self.image_height,self.image_height-j)
                for i in range(self.image_width):
                    # find each pixel in viewport by offsetting 0,0 based on pixel deltas
                    pixel_center = self.pixel00_loc + (self.pixel_delta_u * i) + (self.pixel_delta_v * j)
                    ray_direction = pixel_center - self.camera_center
                    r = ray(self.camera_center,ray_direction)
                    pixel_colour = self.ray_colour(r,world)
                    f.write(pixel_colour.write_colour())
        print("\nDone.") #newline necessary because output stream after progress_indicator call is pointing to end of previous line, not newline
        view_ppm_img(output_ppm_path)

    @staticmethod
    def ray_colour(r: ray, world: hittable) -> color:
        rec = hit_record()
        world_output = world.hit(r, interval(0, math.inf), rec)
        if world_output:
            # normal vector on hit is assumed to be unit length
            # shift the normal vector components (which are between -1 and +1) so that they're between 0 and 2
            # then divide by 2 so they're between 0 and 1
            # this can then be multiplied by 265 to derive a color map based on normals
            return (color(1,1,1) + (world.rec).normal ) / 2

        unit_direction = (r.direction_vec).unit_vector()
        a = 0.5*(unit_direction.y + 1.0)
        # Lerping between (255, 255, 255) which is white to a light shade blue (128, 255*0.7, 255)
        return color(1.0, 1.0, 1.0)*(1.0-a) + color(0.5, 0.7, 1.0)*a

def degrees_to_radians(degrees: float):
    return degrees * math.pi / 180.0

def main():
    #ideal aspect ratio is 16.0 / 9.0
    cam = camera(aspect_ratio=16.0 / 9.0 , image_width=400)

    world = hittables_list([])
    world.add(sphere(point3(0,0,-1),0.5))
    world.add(sphere(point3(0,-100.5,-1),100))
    cam.render(world)

if __name__ == "__main__":
    # for profiling code
    # cProfile.run('main()',sort='tottime')
    main()
            