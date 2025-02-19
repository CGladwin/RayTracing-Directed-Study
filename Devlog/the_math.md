### Math Pre-Requisite Knowledge
- The quadratic formula to solve `ax^2 +bx + c = 0` for x is:
   -  `-b +/= sqrt(b^2 - 4ac)) / 2a`
-  The descrimintant `b^2 - 4ac` tells us how many solutions for x there are in the above formula:
   -  if it's positive, there are 2 solutions
   -  if it's negative, there are no solutions
   -  if it's 0, there's 1 solution
   -  with Pseudocode: `BOOLEAN(b^2 - 4ac > 0) + BOOLEAN(b^2 - 4ac < 0)`
- vectors
  - think of a vector like a bullet travelling in space. 
  - It has magnitude (length)
  - It has direction
  - It took me a while to realize *vectors dont actually have a start and an end point*. you can use a start and end point to describe a vector's length and direction simulaneously, but any vector travelling in that direction, with that length, is treated as identical.
  - Given Points A and B, a vector travelling from points A to B is defined as `B-A`
    - this vector's coordinates are`B.x - A.x, B.y - A.y, B.z - A.z`
- scalars
  - a scalar is a value whose only attribute is its magnitude
  - notably, a scalar can only be 1 number
- norm (euclidean vector norm)
  - the norm of a vector (V) is its length/magnitude
  - it's calculated by the following pseudocode: `SQUARE ROOT(SUM(MAP(V -> V ^ 2)))`
- normal vectors
  - vectors that move perpendicular from an object at a given point. 
- vector dot product
  - takes two vectors (A, B), and produces a scalar based on their length and internal angle
  - with pseudocode, it's `SUM(MAP(A,B -> A*B))`
  - NOTE: `DOT(A,A) = A^2`
  - NOTE: for scalars k,l, `DOT(k,l) = k*l`
- vector cross product
  - takes two vectors (A,B) and produces another vector that's perpendicular to them
  - `CROSS(A,B).x = A.y*B.z - A.z*B.y `
  - `CROSS(A,B).y = A.z*B.x - A.x*B.z `
  - `CROSS(A,B).z = A.x*B.y - A.y*B.x `
- Vector Algebra
  - For any vectors u,v,w, and scalars k,l, we have:
    - Commutative Law: 
      - v+w=w+v
      - "order doesn't matter when adding vectors"
    - Associative Law: 
      - u+(v+w)=(u+v)+w
      - "arbitrary grouping works when adding vectors"
    - Additive Identity:
      - v+0=v=0+v
      - "there exists a zero vector that leaves other vectors unchanged when added to them"
    - Additive Inverse:
      - v+(−v)=0
      - "a vector minus itself cancels out and takes us back to the origin"
    - Associative Law:
      - k(lv)=(kl)v
      - "order doesn't matter when multiplying scalars to vectors, so you can group scalar multiplicands"
    - Distributive Law 1
      - k(v+w)=kv+kw
      - "scalars multiplied to a group of vectors getting summed are multiplied to each addend"
    - Distributive Law 2
      - (k+l)v=kv+lv
      - "vectors multiplied to a group of scalars getting summed are multiplied to each addend"

#### Ray Sphere Intersection
- a ray here is a function `P(t)=A+tb`
  - the Position "P" when given a scalar constant "t" is equal to the ray origin "A" plus the t mult'd by its direction vector "b" (I meant to type "d" for "direction" but I typed "b" and did the entirety of this section like that LOL)
  
- a sphere can be defined in physical space based on its origin C, which contains Cx, Cy, Cz; and radius (r). Given these values, every an arbitrary point P on the sphere (x,y,z) can be calculated. This can be derived because the radius is equal to the length of a straight line drawn from any point on the surface to its center. This line is defined as `C-P`.
  - in math words: `NORM(C-P) = r`  
  - Also intuitively, `NORM(C-P) < r` when P is inside the sphere, and `NORM(C-P) > r` when P is outside.
  - recalling the vector algebra rules mentioned above, we can transform this:
    -  `dot((C - P),(C - P)) = NORM(C-P) ^ 2 = r ^ 2`
  - subbing our ray formula in for P, and applying Associative Law:
    - `dot(((-tb + (C - A))),(-tb + (C - A))) = r^2`
  - applying distributive law (same as polynomial distribution)
    - `dot(-tb,-tb) + dot(-tb,(C-A)) + dot((C-A),-tb) + dot((C-A),(C-A) = r^2`
  - pulling out the scalars:
    -  `-t * -t * dot(b,b) + -t * dot(b,(C-A)) + -t * dot((C-A),b)  + dot((C-A),(C-A)) = r^2`
 -  bringing over r:
    -  `t^2 * dot(b,b) + -2t * dot(b,(C-A)) + (dot((C-A),(C-A)) - r^2) = 0`
 -  the dot product calculations and r are all scalar and known, so the unknown we're solving for is t. Given this, the equation is a quadratic equation. 
       -  here:
          -  `a = dot(b,b)`
          -  `b = -2 * dot(b,(C-A))`
          -  `c = dot((C-A),(C-A)) - r^2`
    -  For now, the number of collisions is the important part, and this can be solved with the discriminant. Lets imagine we're checking for when there's no solutions (since we start with that in the code, explained in Optics):
       -  `0 < (-2 * dot(b,(C-A)))^2 - 4 * dot(b,b) * (dot((C-A),(C-A)) - r^2)`
       - is equivalent to:
         - ` 0 < 4 * (dot(b,(C-A)))^2 - 4 * b^2 * ((C-A)^2 - r^2)`
       - dividing both sides by 4:
         - ` 0 < (dot(b,(C-A)))^2 - b^2 * ((C-A)^2 - r^2)`

#### unit normal vectors 
- they have a magnitude of 1. So for a vector V, `NORM(V) = 1`
- for a sphere:
  - recall `NORM(C-P) = r`
  - the normal vector from any point P would pass through C, therefore we can use `C-P` as our normal vector. It can be converted to unit length easily, since we already know its length is the radius
    - diving the vector by the Norm will mean its length has to be 1, so we're done

#### vector projection
when you project a vector A onto another vector B, you find its component that's parallel to B.
the formula for this is:
- `U.PROJ(V) = (DOT(U,V)/(NORM(V)^2))*V`

#### Snell's law
- Snell's law is how we determine the refraction of our dielectrics
  - `n₁ * sin(θ₁) = n₂ * sin(θ₂)`
  - Here, n₁ and n₂ are the refractive indices of the first and second media, respectively. θ₁ is the angle at which the light ray hits the boundary relative to the normal (the angle of incidence), and θ₂ is the angle at which it travels in the new medium relative to the normal (the angle of refraction).
    - n1, n2, and theta1 are all known at the point where the ray hits the surface, so we need to solve for theta2 (the refraction angle)
  - Shirley et al. use a proof for solving the refracted ray and say "You can go ahead and prove this for yourself if you want". Lets do that!
    1. this proof leverages the following:
      - `sin(θ₂) = (n₁ / n₂) * sin(θ₁)  `
      - `U.PROJ(V) = (DOT(U,V)/(NORM(V)^2))*V`
      - another definition of the dot product: `v⋅w=∥v∥∥w∥cosθ`
        - the vectors here, this can be simplified to `n⋅R=cosθ` since n is normalized, and R has been normalized (when we implemented fuzziness)
      - `sin(θ) = SQRT(1-cos(θ)^2)` 
      - `cos(θ) = SQRT(1-sin(θ)^2)` 
    2. first of all, we're splitting a vector into components that sum up to it, one of which is parallel to the parameter vector its taking (the projection).  The other is perpendicular/orthogonal (the rejection). We'll calculate the rejection first.
      - given by: `U.REJ(V) = U - U.PROJ(V) `
      - `Rprime.REJ(n) = R.REJ(n) * scaling factor` where the scaling factor is given by `sin(θ₂)/sin(θ₁)`
        - this makes logical sense: the ray's perpendicular direction is modified from the incident ray by refraction
      - since `sin(θ₂)/sin(θ₁) = (n₁ / n₂) ` : 
        - `Rprime.REJ(n) = (n₁ / n₂) * R.REJ(n)`
        - `= (n₁ / n₂) * (R - (-R.PROJ(n)))` multiplying R.PROJ(n) by -1 to ensure it's in the correct direction, since n points outward but refraction will point inward
        - `= (n₁ / n₂) * (R + (DOT(R,n)/(NORM(n)^2))*n) ` again, norm(n) is 1
        - `= (n₁ / n₂) * (R + DOT(R,n)*n) `
    3. Now that Rprime.REJ(n) is exclusively written in terms of things we know prior to refraction, we can shamelessly use Rprime.REJ(n) to calculate Rprime.PROJ(n)
       - `Rprime.PROJ(n) = Rprime - Rprime.REJ(n)` 
       - ` NORM(Rprime.PROJ(n)) + NORM(Rprime.REJ(n)) = NORM(Rprime) = 1` since everything is unit length 
       - `SQRT(Rprime.PROJ(n)^2 : scalar) + SQRT(Rprime.REJ(n)^2 : scalar) = 1` and squaring everything gets:
       - `Rprime.PROJ(n)^2 + Rprime.REJ(n)^2 = 1` rearranging, and squaring again:
       - `NORM(Rprime.PROJ(n)) = SQRT(1-Rprime.REJ(n)^2)`
       - `Rprime.PROJ(n) = NORM(Rprime.PROJ(n)) * n` since the projection of Rprime onto n will just be a modification of n by the magnitude of Rprime
       - `Rprime.PROJ(n) = SQRT(1-Rprime.REJ(n)^2) * n`

#### Orthonormal Basis
- "After a few cross products and vector normalizations, we now have a complete orthonormal basis (u,v,w) to describe our camera’s orientation"???
  - an orthonormal basis is a set of vectors that are normalized (have a length of 1) and perpendicular to all other vectors in the set (i.e. the dot product of any 2 distinct vectors in the set will be 0)
  - In 3d space, this set can have up to 3 vectors