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
    - The Computer's Memory has 2 attributes: an address (where it is located in the computer's architecture) and a value stored at that address
    - A pointer is just an address stored as a value
        - `int * pX = &x;` means "pX, which is a pointer to an integer value, is set to the address of x"
            - because the asterisk follows a type, it here modifies the type to be a pointer to that type
        -  `int y = *pX;` means "an integer y is set to the data pointed to by pX"
            - when the asterisk isnt near a type, its used to *dereference* a value, aka retrieve what it points to
        - c++ added reference declarations, with & near a type
            ```cpp
                int a = 10;
                int &a_ref = a;
                a_ref = 9 //Now a will be 9
            ```
            -  effectively, a reference is a const alias to (another name for) the data it points to. This cannot be null, or another reference (aka no indirection). Its advantage is that, since it isnt a "real" value (it doesn't need to be stored in memory to take its address, reassign, etc. because its just an alias to an existing value in memory), it can be optimized to have no impact on memory by the compiler.
            - this allows us to conveniently *pass by reference* , and modify the parameter of a function instead of handing a copy of the parameter to a function (which is *passing by value*)
                - more convenient for function arguments because the variable being passed to the function doesnt need to take up memory distinct from the value we want to modify (unlike a pointer)
                - also: references don't need to be validated as not null
        - in c++ `obj->x = 10` is syntax to dereference an object "obj" and access its member x
- smart pointers
- operator overloading
  - operator overloading is how C++ allows for runtime polymorphism 
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
    - this has a performance cost. Inlined functions, when possible, get inserted to where they're called at compile time, so that this control flow switcheroo doesn't happen.
- constexpr's
- templates (compile-time metaprogramming)
  - C++ allows dispatch on user defined types. I think it only allows runtime dispatch on builtin types
- almost always auto 
- the key data structure; explaining "std::vector<u_int8_t>&"
- std::clog
- A good makefile