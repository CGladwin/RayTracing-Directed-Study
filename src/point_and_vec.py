# unit 3: the vec3 class
import numpy as np
import math
import numpy as np
from pydantic import BaseModel
class base_vec3():
    x: float
    y: float
    z: float
    def __init__(self, x,y,z) -> None:
        self.x = x
        self.y = y
        self.z = z

    @classmethod
    def from_list(cls,coordinates_list):
        return cls(coordinates_list[0],coordinates_list[1],coordinates_list[2])

    def as_list(self):
        return [self.x,self.y,self.z]
    
    def __add__(self, other):
        return vec3(self.x + other.x, self.y + other.y,self.z + other.z)

    def __sub__(self, other):
        return vec3(self.x - other.x, self.y - other.y,self.z - other.z)

    def __mul__(self, other: float | int):
        return vec3(self.x * other, self.y * other,self.z * other)

    def __truediv__(self, other: float | int):
        return vec3(self.x / other, self.y / other,self.z / other)

    def __str__(self) -> str:
        return "%f %f %f" % (self.x,self.y,self.z)

class vec3(base_vec3):
    def length_squared(self) -> int:
        return self.x**2 + self.y**2 + self.z**2
    
    def norm(self) -> complex:
        # equivalent to np.linalg.norm()
        return math.sqrt(self.length_squared())

    def cross(self, other):
        return vec3((self.y * other.z) - (self.z * other.y),
                    (self.z * other.x) - (self.x * other.z),
                    (self.x * other.y) - (self.y * other.x))
    
    def dot(self, other) -> float:
        return (self.x * other.x) + (self.y * other.y) + (self.z * other.z)
    
    def unit_vector(self) :
        return self / self.norm()
    
    def to_color(self):
        return color(self.x,self.y,self.z)
        
class color(vec3):
    @classmethod
    def to_color(cls,base: vec3):
        cls.x = base.x
        cls.y = base.y
        cls.z = base.z
        return cls

    def __str__(self) -> str:
        return "%d %d %d\n" % (self.x*256,self.y*256,self.z*256)
    
    def write_colour(self) -> str:
        # note: original write_colour function sends string to output
        # python convention is to enclose file writing in the with open() context so it seems more scalable to return a string instead
        # output = int(np.average(rgb_byte_output))
        return "%d %d %d\n" % (self.x*256,self.y*256,self.z*256)
    
    def write_colour_array(self) -> list:
        return [int(256 * x) for x in self.as_list()]

class point3(vec3):
    pass
    #original code has vec3 and point3 as aliases of the same classes, used to visualize the geometric concepts
    #IE a vector (has a direction) vs a point in space
    #this seems like bad practice will look for ways to refactor

if __name__ == "__main__":
    a = point3(4,5,6)
    print(a)
    b = point3(1,2,3)
    c = a.dot(b)
    print(a.length())

