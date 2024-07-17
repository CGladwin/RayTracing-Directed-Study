# unit 3: the vec3 class
import math
import numpy as np

# class to manage intervals of floats
class interval():
    def __init__(self, min: float,max: float) -> None:
        self.min = min
        self.max = max
    
    def size(self):
        return self.max - self.min
    
    def contains(self,x: float):
        return self.min <= x and x <= self.max
    
    def surrounds(self,x: float):
        return self.min < x and x < self.max
    
    def clamp(self,x: float):
        if x < self.min:
            return self.min
        if x > self.max:
            return self.max
        return x

empty = interval(math.inf,-math.inf)
universe = interval(-math.inf,math.inf)

class base_vec3():
    def __init__(self, x,y,z) -> None:
        self.x = x
        self.y = y
        self.z = z

    @classmethod
    def from_list(cls,coordinates_list):
        return cls(coordinates_list[0],coordinates_list[1],coordinates_list[2])
    
    @classmethod
    def from_vec3_like(cls,other):
        return cls(other.x,other.y,other.z)
    
    @classmethod
    def from_random(cls):
        return cls(np.random.rand(),np.random.rand(),np.random.rand())
    
    @classmethod
    def from_rand_range(cls,max,min):
        return cls(np.random.uniform(max,min),np.random.uniform(max,min),np.random.uniform(max,min))

    def as_list(self):
        return [self.x,self.y,self.z]
    
    def __add__(self, other):
        return self.__class__(self.x + other.x, self.y + other.y,self.z + other.z)

    def __sub__(self, other):
        return self.__class__(self.x - other.x, self.y - other.y,self.z - other.z)

    def __mul__(self, other: float | int):
        return self.__class__(self.x * other, self.y * other,self.z * other)

    def __truediv__(self, other: float | int):
        return self.__class__(self.x / other, self.y / other,self.z / other)

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
    
    # returns vector of length 1
    def unit_vector(self) :
        return self / self.norm()
    
    @staticmethod
    def random_in_unit_sphere():
        while True:
            p = vec3.from_rand_range(-1,1)
            if p.length_squared() < 1:
                return p
    @classmethod      
    def random_unit_vector(cls):
        return cls.unit_vector(vec3.random_in_unit_sphere())
    
    @classmethod
    def random_on_hemisphere(cls,normal):
        on_unit_sphere = cls.random_unit_vector()
        if on_unit_sphere.dot(normal) > 0:
            return on_unit_sphere
        else:
            return on_unit_sphere * -1
        
class color(vec3):
    def __str__(self) -> str:
        return "%d %d %d\n" % (self.x*256,self.y*256,self.z*256)
    
    def write_colour(self) -> str:
        # note: original write_colour function sends string to output
        # python convention is to enclose file writing in the with open() context so it seems more scalable to return a string instead
        # output = int(np.average(rgb_byte_output))
        intensity = interval(0,0.999)
        return "%d %d %d\n" % (intensity.clamp(self.x)*256,
                               intensity.clamp(self.y)*256,
                               intensity.clamp(self.z)*256)
    
    def write_colour_array(self) -> list:
        return [self.x*256,self.y*256,self.z*256]

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

