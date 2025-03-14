### The toolchain and auxillary programs
I will not explain:
- compiling c++ code

Python Scripts

External C++ Libs
- JSON.hpp by nlohmann
  - This program was what the internet recommended for parsing JSON in C++
  - Unfortunately, this project sort of clashes with any language without built in support for JSON, like c++
    - A scene object, like a sphere, has a complex series of inheritance and dependencies. On top of that, most classes in this project were not designed to be default constructible and specified after, but rather specified with a custom constructor
    - Due to this, my options to use the library ideomatically (i.e. with the ".get" function it provides) are `1` move tons of the scene object modelling code (e.g. everything inheriting hittable.hpp, everything inheriting material.hpp) into the JSON.hpp namespace (which seems like a bad idea) or `2` make objects default constructible and create a to_json function that deserializes a json file using this default constructor.
    - I discovered an alternative: create a from_json() function that reads the json string elements to declare the arguments of the custom constructors, then returns a shared-pointer owned hittable object. It's not ideomatic, and it might perform worse, but it's a better fit for the codebase

I learned about:
- GDB
- CMAKE
- Doxygen for documentation
- Initially using python
- Setting up .vscode for debugging c++
- profiling code:
  - valgrind
  - flamegraph