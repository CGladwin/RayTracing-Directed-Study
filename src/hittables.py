from dataclasses import dataclass
from point_and_vec import *
from ray_class import *
from abc import ABC, abstractmethod

class hit_record():
    def __init__(self) -> None:
        pass

    def set_face_normal(self,r: ray, outward_normal: vec3) -> None:
        # Sets the hit record normal vector.
        # NOTE: the parameter `outward_normal` is assumed to have unit length.
        self.front_face = (r.direction_vec).dot(outward_normal) < 0
        if self.front_face:
            self.normal = outward_normal
            return
        self.front_face = outward_normal * -1

class hittable(ABC):
    @abstractmethod
    def hit(r: ray, ray_tmin: float, ray_tmax: float, rec:  hit_record) -> bool:
        pass

class hittables_list(hittable):
    def __init__(self,objects_list: list[hittable]) -> None:
        self.objects_list = objects_list
    
    def add(self,object: hittable) -> None:
        self.objects_list.append(object)
    
    def hit(self,r: ray, ray_tmin: float,ray_tmax: float, rec: hit_record) -> bool:
        temp_rec = hit_record()
        hit_anything = False
        closest_so_far = ray_tmax
        for object in self.objects_list:
            if object.hit(r, ray_tmin, closest_so_far, temp_rec):
                hit_anything = True
                closest_so_far = temp_rec.t
                rec = temp_rec
        return {"hit_anything":hit_anything,"rec":temp_rec}

class sphere(hittable):
    def __init__(self,center: point3,radius: float) -> None:
        self.center = center
        self.radius = max(0,radius)

    def hit(self,r: ray, ray_tmin: float,ray_tmax: float, rec: hit_record) -> bool:
        # vector from center of sphere to camera, used to declare where the sphere exists in virtual 3d space
        # origin_to_center.dot(origin_to_center) == (Cx - x)^2 + (Cy - y)^2 + (Cz - z)^2 = r^2 
        # In the above C is a point in space, the other components are the coords of the center of the sphere
        oc: vec3 = self.center - r.origin_point
        # coefficients in the equation at^2 + bt + c = 0, where t is the scalar multiple of the ray's direction in the equation P(t) = Q + td
        # note a.dot(a) == a.length_squared()
        a = (r.direction_vec).length_squared()
        #  h = b/-2, used as a simplification of quadratic formula
        h = (r.direction_vec).dot(oc)
        c = oc.length_squared() - self.radius**2
        # find the number of solutions
        discriminant = h*h - a*c
        
        if (discriminant < 0):
            # no solutions, ray doesn't hit sphere
            return False
        # do expensive math.sqrt operation after ensuring ray hits sphere
        sqrtd = math.sqrt(discriminant)
        # Find the nearest root that lies in the acceptable range.
        root = (h - sqrtd) / a
        if (root <= ray_tmin or ray_tmax <= root):
            root = (h + sqrtd) / a
            if (root <= ray_tmin or ray_tmax <= root):
                return False

        # note: unit vector values are between -1 and 1
        rec.t = root
        # get point on surface of sphere (may or may not be facing camera)
        rec.p = r.at(rec.t) 
        # find the unit vector pointing normal to the surface of the sphere at point t
        outward_normal: vec3 = (rec.p - self.center) / self.radius
        # if the normal is in the same direction as the ray, flip its direction
        rec.set_face_normal(r, outward_normal)

        return True