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
The book passage explaining the metaphor of how this is achieved is kind of confusing, but this is how I understand it: imagine a unit sphere U tangent to the ray's point of contact p on the surface of a sphere. There are fewer possible points within U that are away from the normal than towards it. This can be seen by looking at possible chords (lines drawn from points on U). The shortest chords point away from the normal (indicating they're less likely to be sampled), and the longest chord possible is the same direction as the normal vector (indicating it's more likely to be sampled)

#### Gamma Correction
The book mentions "The (current) picture is very dark, but our spheres only absorb half the energy of each bounce, so they are 50% reflectors". explainer: "If a ray bounces off of a material and keeps 100% of its color, then we say that the material is white. If a ray bounces off of a material and keeps 0% of its color, then we say that the material is black."

"almost all computer programs assume that an image is “gamma corrected” before being written into an image file. This means that the 0 to 1 values have some transform applied before being stored as a byte"
- the image viewer used expected an image in *gamma space*, with a transform applied to the bytes, but we gave it one in *linear space*
- to transform it to gamma space, we have to raise our rgb values to the exponent 1/gamma. We use gamma = 2, which means we have to take the square root of our color values

#### Modeling Light Scatter and Reflectance
- **Albedo**: literally "whiteness", indicates reflectance (ranging from 0 to 1). Shirley et al use albedo for their diffuse material such that the lambertian material always scatters and attenuates (shortens) incoming ray. Other methods:
  -  rays are sometimes scattered based on a probability of 1-R (less reflective materials are more likely to absorb incoming rays)

#### Mirrored Light Reflection
- polished materials scatter light more deterministically via reflection
    1. when reflection happens to an incoming ray v, we decompose it to 2 parts, one parallel (tangent) to the surface, and one normal to the surface (here called b)
    2. this parallel component doesn't transform, since the ray continues in that direction relative to the surface
    3. b does transform in the outgoing vector o, however, into -b (since the vector is flipped)
    4. therefore the change (delta) from v.b to o.b is `new - old = -b -(b) = -2b`
        - this makes logical sense. If the change applied was -b, then the vector would become [0,0,0]
-  This transformation of the incoming vector v here is v - 2(v.PROJ(n)), where n is the surface norm
  - consider that `(NORM(n)^2)` here is `1^2` since n is normalized already
- note: shirley et al. here says that the outgoing vector is v + 2b, but I think that's misleading

#### Fuzzy Reflection
- Fuzzy Reflection is generated by taking a point on the outgoing ray after reflection o, creating a small sphere (its radius corresponds to how fuzzy we want the material to be, and is predefined), selecting a random point on the sphere `po`, and declaring that the outgoing vector now passes through `po`
  - "The catch is that for big spheres or grazing rays, we may scatter below the surface. We can just have the surface absorb those."
  - fuzziness also requires that we normalize the reflected ray, since it can vary arbitrarily and makes this calculation difficult

#### Dielectrics
"Clear materials such as water, glass, and diamond are dielectrics. When a light ray hits them, it splits into a reflected ray and a refracted (transmitted) ray. We’ll handle that by randomly choosing between reflection and refraction"
- a reflected ray hits the surface, and moves out into space in a new direction
- a refracted ray hits the surface, and keeps moving inside the object, in a slightly different direction (i.e. the ray bends). This explains the optical illusions that a glass of water can create (e.g. enlarging things on the other side)
- "The amount that a refracted ray bends is determined by the material's refractive index"
  - Glass has a refractive index of something like 1.5–1.7
  - diamond is around 2.4
- "When a transparent material is embedded in a different transparent material, you can describe the refraction with a relative refraction index: the refractive index of the object's material divided by the refractive index of the surrounding material. For example, if you want to render a glass ball under water, then the glass ball would have an effective refractive index of 1.125. This is given by the refractive index of glass (1.5) divided by the refractive index of water (1.333). "
  - what is the refractive index of a hollow glass ball?
- **Snell's law** is how we determine the refraction of our dielectrics (see: The Math.md)
  - a side effect of Snell's law: "ray angles for which no solution is possible using Snell's law."
    - given `n₁ * sin(θ₁) = n₂ * sin(θ₂)` 
    - given `sin(θ₂) <= 1` (can be demonstrated by looking at a sine wave)
    - therefore  `n₁ / n₂ * sin(θ₁) <= 1`
      - this theoretically does not always hold true, and when that happens we have to reflect the ray
    - initially, we were always refracting the ray, but we have to sometimes reflect the ray based on whether the above equation holds true or not. If i'ts not held true, we reflect
      - Choosing to reflect the ray  internally instead of allowing it to refract replicates the phenomenon of  **"Total internal reflection"**, which is when light reflects back through a material before exiting it due to it's angle of approach and the material having a higher refractive index than what exists outside
      - (e.g. the surface of water can become an inverted mirror when looked at the correct angle beneath it)
      - however, a glass sphere in air will never produce total internal reflection
    - instead, to demonstrate a similar effect, we do total external reflection
      - we can imagine the dielectric material has lower RI than its surroundings (like an air bubble in water), and observe that most rays refract, while some incident rays on the edges reflect
      ```cpp
        //this represents a material with a lower RI than its surroundings
        auto material_left   = make_shared<dielectric>(1.00 / 1.33);
      ``` 
  - simulating a hollow glass sphere is #1 relatively easy from here and #2 allows us to treat the default RI as 1 (like air).
    - generate a "glass" sphere (a sphere with our always-refracting dieletric material and an RI = 1.5)
    - generate another sphere inside it (same center point, slightly smaller radius), and assume its internal RI is 1, and its outer RI is 1.5 (since it's encased in glass)
      - therefore the RI ratio is 1/1.5
  - real air to glass (or similar high RI) behaviour with realistic reflective properties can be simulated using **Schlick's approximation**:
    - `R(θ)=R0​+(1−R0​)(1−cosθ)^5` Where:
      - R(θ) is the reflectance (how much light is reflected) at an angle θ.
      - R0 is the reflectance when an incoming ray is travelling parallel to the normal vector on the surface.
        - derived by `((n1 - n2)/(n1 + n2))^2` where n1 is the outer material RI and n2 is the inner material RI
      - θ is the angle between the view direction and the surface normal.
    - when this is applied, you can see that there is a slight reflection of other spheres in the environment within a glass sphere

#### Camera Adjustments
- **FOV**
  - in our case, we're allowing the user to adjust the vertical FOV (the height of the viewport in y space)
    - Shirley et al. "usually specify it in degrees and change to radians inside a constructor"
  - the area enclosed from the ray origin [0,0,0] to the vertical ends of the viewport, when looked at the side, can be visualized as 2 right angled triangles. This means, if the angle from the top to the bottom vertically is θ, then we can use `tan(θ/2) = opposite/adjacent` to determine the viewport height of one triangle
    - adjacent is the focal length (distance from ray origin to camera), so `tan(θ/2) * focal_length = viewport_height/2` 
    - therefore  `tan(θ/2) * focal_length * 2 = viewport_height`
- Adjustable camera position
  - we can allow the camera to freely move in the scene, if a few things are specified:
    1.  the camera center, now called "lookfrom"
    2.  the viewport center, now called "lookat" (remember, the ray cast from lookfrom to this point will be perpendicular to the viewport)
    3.  the orientation of the viewport (which way is "up" in space)
        - for this, we can treat "up" as the same as everything else
        ```js
          vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction
        ``` 
        - this results in an image that's horizontally level (our viewport pixel rows are parallel to the world's x-axis )

#### Bound Volume Hierarchies
- [here](https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/) is a deeper dive into bound volume hierarchies
- The core idea of Bound Volume Hierarchies appears to be similar to the binary search algorithm on a sorted array.
  - Instead of searching the whole array linearly for the value you want, you subdivide the array, check if the value you want would fall within the elements bounding it, and if so, continue subdividing recursively.
  ```py
  if ray hits bounding object
        return whether ray hits bounded objects
    else
        return false 
  ```
- this bounding object can probably be a primitive that's easy to calculate intersections on, like a sphere 
- The rule is that the children of bound volumes must be totally enclosed, but sibling bound volumes may overlap, and sibling objects or volumes have no concept of order
- the most optimized technique is for bounding objects to be nested inside parent bounding volumes. These parents totally enclose their children volumes
- Axis-Aligned Bounding Boxes (dividing objects into boxes that are oriented parallel to the x,y, and z axis in space) works well
  - these boxes can be treated as "slabs" only defined by intervals in space
    - In 3D, a slab is the space between two parallel planes
      - we only need 3 slabs to define an AABB 
    - an interval here is distinct from a vector, in that it has magnitude defined by 2 endpoints, but not direction
- "if a ray intersects the box bounded by all pairs of planes, then all t-intervals will overlap"
  - i.e. if a ray intersects a box, the equation `Ray_point(constant_t)=Origin_Point + constant_t * direction_Vec` will have some solution t, where Ray_point(t).x, Ray_point(t).y and  Ray_point(t).z is within its interval, 
- The pseudocode of what we're trying to do (by breaking down a ray into its axial components and finding if they land within an interval):
  ```CPP
    interval_x ← compute_intersection_x (ray, x0, x1)
    interval_y ← compute_intersection_y (ray, y0, y1)
    interval_z ← compute_intersection_z (ray, z0, z1)
    return overlaps(interval_x, interval_y, interval_z)
  ```  
- some caveats:
  - we need to handle when a ray is travelling in a negative direction, and want to return the inverse of an interval we planned for (e.g, [7,3] instead of [3,7])
    - When computing intersections, you need to ensure that tx0 is the smaller value and tx1 is the larger value. If they are reversed, you’ll need to swap them to maintain consistency
  - because we're breaking down the ray formula into its components (e.g. `Ray_point(constant_t).x = Origin_Point.x + constant_t * direction_Vec.x`), its possible for a direction_Vec component to be 0 if the ray does not move in that axis (which results in dividing by 0 when isolating for t!). Its also possible for the ray origin to lie on a slab, if true along with the former, results in 0/0 = NaN!
  - When dx = 0, you can use min and max operations to correctly determine whether the ray intersects the slab. For example:
    - If both tx0 and tx1 are +∞, the ray is outside the slab and moving away from it.
    - If both are -∞, the ray is outside the slab and moving toward it (but still parallel, so no intersection).
    - If one is +∞ and the other is -∞, the ray lies entirely within the slab.
  - For now, the book asks us to arbitrarily decide whether the rays whose origins are on a slab plane are hit or not
- I've not yet tested the results empirically of the speedup between BVH and the more naive approach, but casual guesswork seems to show that, for 3 or fewer spheres, the naive approach is much faster, and for 4 spheres, they're about the same (with BVH having significantly less change in time-cost than naive)