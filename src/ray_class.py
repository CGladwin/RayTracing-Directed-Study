from point_and_vec import point3, vec3
class ray():
    def __init__(self, origin : point3, direction: vec3) -> None:
        self.origin_point = origin
        self.direction_vec = direction
    def at(self,t:int):
        return self.origin_point + (t * self.direction_vec)
    

