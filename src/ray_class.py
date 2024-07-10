from point_and_vec import point3, vec3
class ray():
    def __init__(self, origin : point3, direction: vec3) -> None:
        self.origin_point: point3 = origin
        self.direction_vec: vec3 = direction
    def at(self,t:int):
        return self.origin_point + (self.direction_vec * t)
    

