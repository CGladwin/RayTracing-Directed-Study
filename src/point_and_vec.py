# unit 3: the vec3 class
import numpy as np
import math
import numpy as np
class base_vec3():
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
        norm = self.norm()
        return vec3(self.x / norm, self.y / norm, self.z / norm)
    
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
    
class np_vec3(np.ndarray):
    def __new__(cls,input_array=None):
        if input_array is None:
            return np.zeros(3).view(cls)
        if len(input_array) != 3:
            raise ValueError("incorrect number of args, 3 required")
        if type(input_array) == list:
            return np.array(input_array).view(cls)
        return input_array.view(cls)
    
class old_vec3(np_vec3):
    def x(self):
        return self[0]
    
    def y(self):
        return self[1]
    
    def z(self):
        return self[2]
    
    def norm(self) -> int:
        return np.linalg.norm(self)
    
    def length_squared(self) -> int:
        # equivalent to np.linalg.norm()
        return sum([x**2 for x in self])
    
    def length(self) -> complex:
        return math.sqrt(self.length_squared())

    def cross(self, other):
        return vec3(np.cross(self,other))
    
    def dot(self, other) -> int:
        return np.dot(self,other)
    
    def unit_vector(self) :
        return vec3([x / self.norm() for x in self])
    
class old_color(vec3):
    def write_colour(self) -> str:
        # note: original write_colour function sends string to output
        # python convention is to enclose file writing in the with open() context so it seems more scalable to return a string instead
        rgb_byte_output = [256 * x for x in self]
        # output = int(np.average(rgb_byte_output))
        return "%d %d %d\n" % (rgb_byte_output[0],rgb_byte_output[1],rgb_byte_output[2])
    def write_colour_array(self) -> list:
        return [int(256 * x) for x in self]

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

