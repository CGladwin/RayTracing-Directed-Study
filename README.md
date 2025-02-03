Going through Ray-tracing in one weekend, and making tweaks here and there. Then, building a web app to let users create scenes for raytracing.

## GOALS

- Understand the mathematics behind fundamental graphics programming concepts
- Gain familiarity with Full-Stack Development
- Learn best practices and optimizations for low-level programming, with C++

IDEAS:

- implement multithreading with std::threads, allowing user to see each thread running simultaneously
- reduce inheritance in project to prefer composition (maybe take a "data oriented design" approach)
- make small optimizations based on other approaches to the project (e.g. use less shared pointers and virtual functions)
- add bound volume heirarchy
- add Doxygen-style comments/documentation
- containerize project with Docker
- Build frontend that sends json (and linked obj files) to backend to render image

The goal of this project is to become a backend of a web application that uses a web-gl framework to let users design a scene, then call the raytracer, hosted on a custom web server

Note: .vscode directory must be at root of project when in vs code (embedded run/debug tools don't work otherwise)
