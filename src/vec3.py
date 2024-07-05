# unit 3: the vec3 class
import numpy as np
import math
# class vec3():
#     def __init__(self, coordinate_list : None | list[int] = None) -> None:
#         if coordinate_list is None:
#             self.e = np.zeros(3)
#             return
#         if len(coordinate_list) != 3:
#             raise ValueError("incorrect number of args, 3 required")
#         if type(coordinate_list) == list:
#             self.e = np.array(coordinate_list)
#             return
#         self.e = coordinate_list
    
#     def x(self):
#         return self.e[0]
#     def y(self):
#         return self.e[1]
#     def z(self):
#         return self.e[2]
    
#     def __add__(self, other):
#         return vec3(self.e + other.e)

#     def __sub__(self, other):
#         return vec3(self.e - other.e)

#     def __mul__(self, other):
#         return vec3(self.e * other.e)

#     def __truediv__(self, other):
#         return vec3(self.e / other.e)
    
#     def __str__(self):
#         return str(self.e)
    
class vec3(np.ndarray):
    def __new__(cls,input_array=None):
        if input_array is None:
            return np.zeros(3).view(cls)
        if len(input_array) != 3:
            raise ValueError("incorrect number of args, 3 required")
        if type(input_array) == list:
            return np.array(input_array).view(cls)
        return input_array.view(cls)
    
class point3(vec3):
    def x(self):
        return self[0]
    
    def y(self):
        return self[1]
    
    def z(self):
        return self[2]
    
    def length_squared(self) -> int:
        return sum([x**2 for x in self])

    def length(self) -> complex:
        return math.sqrt(self.length_squared())

    def cross(self, other: vec3) -> vec3:
        return vec3(np.cross(self,other))
    
    def dot(self, other: vec3) -> int:
        return np.dot(self,other)
    
    def unit_vector(self) :
        return vec3([x / self.length() for x in self])
    
    def write_colour(self,output_path):
        rgb_byte_output = [256 * x for x in self]
        with open(output_path,"w+") as f:
            f.write("%d %d %d\n" % (rgb_byte_output[0],rgb_byte_output[1],rgb_byte_output[2]))
        

if __name__ == "__main__":
    a = point3([4,5,6])
    print(a)
    b = point3([1,2,3])
    c = a.dot(b)
    print(a.length())

