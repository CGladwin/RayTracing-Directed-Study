### [](https://)A high level explanation of what's going on

In the real world, we see by light being emitted from light-sources, the rays bouncing off objects, and then hitting our eyeballs. While they're bouncing around, their energy gets absorbed by the objects they hit, which determines what colour the object is percieved as.

At its core, a ray tracer does the reverse of this process; it sends rays (analogous to light) through pixels (small points in space, analogous to our eyes) and computes the color seen in the direction of those rays. This allows for fewer rays being needed to visualize a scene than if the rays were casted from light sources directly (since not all of those rays would be used in the final scene).

The involved steps are

- Calculate the ray moving from the "camera" through the “eye” pixels in the viewport,
- Determine which objects the ray intersects, and
- Compute a color for the closest intersection point.

#### What (and where, and how) is the camera?

The camera here is just a point that we're sending all the rays from.

We're going to set it as the origin, [0x,0y,0z]

The vector from the camera center to the viewport center will be orthogonal to the viewport.

[here's a visualization from the book](https://raytracing.github.io/images/fig-1.03-cam-geom.jpg)

#### btw, aspect ratio

The aspect ratio is the ideal ratio of pixels in the image's height, compared to its width. An extremely common aspect ratio is 16:9, which would mean we want 1.7778 pixels vertically for a width of 1. This isn't possible; you can't have a fractional pixel.
