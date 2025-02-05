### A high level explanation of what's going on

In the real world, we see by light being emitted from light-sources, the rays bouncing off objects, and then hitting our eyeballs. While they're bouncing around, their energy gets absorbed by the objects they hit, which determines what colour the object is percieved as.

At its core, a ray tracer does the reverse of this process; it sends rays (analogous to light) through pixels (small points in space, analogous to our eyes) and computes the color seen in the direction of those rays. This allows for fewer rays being needed to visualize a scene than if the rays were casted from light sources directly (since not all of those rays would be used in the final scene).

The involved steps are

- Calculate the ray moving from the "camera" through the “eye” pixels in the viewport,
- Determine which objects the ray intersects, and
- Compute a color for the closest intersection point.

Move on to the next eye pixel, starting in the top left eye pixel and going to the bottom right.

#### What (and where, and how) is the camera?

The camera here is just a point that we're sending all the rays from.

We're going to set it as the origin, [0x,0y,0z]. 3d space from here is described using right handed coordinates. Do a thumbs up with your right hand, point forward with your pointer and middle, then fold your middle halfway down (making a reverse L with the pointer). Here, your palm is the camera point, your thumb is the y axis, and your pointer and middle are the x and z axis.

#### the viewport
The viewport is a slice of space where viewport pixels (or eye pixels) can exist. Why do we define a viewport containing a grid of pixels, and not just a grid of pixels? This viewport helps the programmer to define the *density* of eye pixels, by defining their internal width in the viewport and the size (total number of pixels) of the image being taken.

The viewport pixels, by default, are arranged like a slice of space in the x and y axis; any vector drawn from one point to another (which you can call an internal viewport vector) would have the same z value. 

"The vector from the camera center to the viewport center will be orthogonal to the viewport" is something Shirley et. al. Wrote that I had to reread a few times. Orthogonal here means the same thing as "perpendicular".

another way to put it: if the main camera ray was a laser, and the viewport was a rectangular mirror, this ray would bounce straight back to the source.

[here's a visualization from the book](https://raytracing.github.io/images/fig-1.03-cam-geom.jpg)

#### btw, aspect ratio

The aspect ratio is the ideal ratio of pixels in the image's height, compared to its width. An extremely common aspect ratio is 16:9, which would mean we want 1.7778 pixels vertically for a width of 1. This isn't possible; you can't have a fractional pixel.

#### Surface Normals
A surface normal is a normal vector that originates from some point on the sphere. because its perpendicular to the sphere, it would pass through the radius if it was infinitely long.

Shirley et. al. make 2 key design decisions for Surface Normals:
- they are converted to unit length
- they are pointing outwards (away from the Sphere Center) always

#### Ray Sphere Intersection
In "the math", we see how to calculate ray-sphere intersection. There, we learn that the only thing we need to solve for is "t", the scalar constant in the ray equation. Here, we explain how it affects the control flow of the function. Initially, when we don't have any textures, it'll look like this:

Was there any hits? -> no -> return false
|-> yes -> return true

To display objects correctly, rays should only interact with the first point on an object they intersect with. So later, it will be:

Was there any hits? -> no -> return false
|-> yes -> what is the t of the nearest hit?

In practice, this is susceptible to precision-based errors (the program cannot be infinitely precise), noticeably when t approaches 0. So it will ultimately look like this :

Was there any hits? -> no -> return false
|-> yes -> what is the t of the nearest hit?
|-> some t -> is t far enough from 0?

where we set some cutoff point.

#### Front Faces Versus Back Faces
We'll need to know whether the ray is on the inside or outside of the objects it hits, since rays can pass through some materials (like glass). The book chooses to figure this out by setting normal vectors to always point away from the sphere's center. So, "if the ray and the normal face in the same direction, the ray is inside the object, and if the ray and the normal face in the opposite direction, then the ray is outside the object" (Shirley et. al.). The dot product can be use to figure out which case is which.

#### Antialiasing

#### Simple diffuse material
In the branch "diffuse_material" I've implemented the simple diffuse material mentioned by Shirley et al. Here, rays bounce in a random direction away from the surface of an object.  

#### Lambertian Material
Lambertian Material, visually, looks like a matte surface. This is done by making sure a reflected ray is most likely to scatter towards the surface normal.

