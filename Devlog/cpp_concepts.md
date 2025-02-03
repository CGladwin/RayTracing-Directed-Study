### C++ Concepts leveraged
Pre-requisites:
- Understand the basics of OOP
- Understand vaguely how a compiled language works
  - compiling to byte code
  - linking object files into an executable
  - executing the program
- understand the difference between compile-time and runtime code execution

I learned about:
- what the hell is the deal with & and * ?
- smart pointers
- operator overloading
  - operator overloading is how C++ allows for runtime 
- OOP
    - member initializer lists
- external libraries
  - stb_image_write
- pragma #once
- virtual functions
- **inline functions**
  - the big picture: inline functions are expanded during compile-time, reduce function calls
  - what that means:
    - normally when a function is called at runtime:
      - the CPU stores the memory address of the instruction following the function call (to continue the program afterwards)
      - it copies the function parameters on the stack
      - finally it transfers control to the specified function
    - this has a performance cost. Inlined functions, when possible, get inserted to where they're called at compiile time, so that this control flow switcheroo doesn't happen.
- constexpr's
- templates (compile-time metaprogramming)
  - C++ allows dispatch on user defined types. I think it only allows runtime dispatch on builtin types
- almost always auto 
- the key data structure; explaining "std::vector<u_int8_t>&"
- std::clog
- A good makefile