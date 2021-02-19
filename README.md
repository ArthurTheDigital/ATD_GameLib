# ATD Game Library
This is my personal replacement of SFML.

## Version
0.1.0

## Languages
* C++11 (main)
* GLSL v1.3.0 (shaders)
* GNU Make (build scripts)
* Bash (build scripts)

## Code style
1. Constants (in enums too!) in CAPS, variables, functions and methods in 
camelCase, classes in PascalCase, in order to avoid naming conflicts with 
third-party libs use `M_`, `m` or `M` prefix respectively. Static 
functions and variables in camelCase with `_` prefix.
2. Private fields shall have `m_` prefix.
3. If constructor/setter parameter name conflict with field name, use `n_` 
prefix for that parameter (standing for 'new').
4. For parameters of type `void *`, `const void *`, that are about to be 
casted to a valid pointer of certain type, use prefix `r_` (standing for 
'raw'). Often used in callbacks.
5. Trivial getters, that return valueName by constant link shall be 
inlined and named `valueName()`. If getter is non-trivial, e.g. requires 
significant calculations and thus cannot be inlined, it should be named 
`getValueName()`. It makes easier to see, how optimal the code is. 
Getters, that usually just return cached value, but also may calculate the 
value if the cache is dirty, shall follow the first name pattern 
(`valueName`). All setters are to be named `setValueName(...)`.
6. Methods that return a modified version of the object itself shall not 
follow the getter naming, even if not inlined and require computation.
7. No macro definitions in header files. For constant values use 
`static const` instead, they are way better, because they are namespaced. 
The exceptions are `Core/Debug.hpp` and `Graphics/GlCheck.hpp`.
8. Time constants/parameters/variables must be named with time type at the 
end (hours(h), minutes(m), seconds(s) or milliseconds(ms)), e.g 
`WAIT_INTERVAL_H`, `sleepTimeMs`, etc..
9. Same as for angles. In this library there are three types of them: 
degrees(deg), radians(rad) and fraction-of-circle(frc). Fraction-of-circle 
is more preferrable, because it is more convenient for game logic.
10. Unconditional loops are done with `while (1) { ... }`.
11. Doxygen commands start with at-sign (@).

## Modules
The structure of the library shall be modular. Module dependencies are 
tree-like.

=----------=
|   Core   |
=----------=
    |  \____________
    |               \
    v                v
=----------=    =----------=
| Graphics |    |   Ansi   |
=----------=    =----------=
     |
     |
     v
=----------=
|  Window  |
=----------=

### General
* **TODO:** Refactor: functions and methods -> camelCase.
* **TODO:** Basic documentation.
* **TODO:** Audio module.
* **TODO:** Try using Doxygen.
* **TODO:** Cache resolved headers while building the project.
* **TODO:** Refactor build system for tests and apps.

### Core module
* Basic math: 2D, 3D, 4D vectors, 4D matrixes.
* Texturing math: texturing algorithms, 2D rectangles.
* Fs path for comfortable filesystem operations.
* Tags operations.
* Unicode string operations and cast to/from UTF-8.
* Loadable base class.
* Debug, AutoTest and LogWriter classes for generic debug.
* Transform2D/Projection2D classes.
* Transform3D/Projection3D classes.
* **TODO:** My own exception class. Should have two strings: short and 
detailed.

### Ansi module
* Advanced ANSI colored output in terminal.
* **TODO:** Add more things for automation.
* **TODO:** Debug Ansi class for tracking Loadable tree.

### Graphics module
* OpenGL functions, types and constants wrapping.
* Images, that can be saved and loaded, modifyed and saved as .png, .jpeg 
and .gif (only 1st frame).
* AnimatedGif for loading and saving .gif animations.
* **TODO:** Test AnimatedGif.
* Unmodifyable OpenGL textures, that are loaded from images.
* GlFrameBuffer class.
* Image, obtained from texture.
* GLSL shaders, that can be loaded, and assigned uniforms.
* **TODO:** Shader constructor fails if called before Window constructor. 
This is why I use smart pointers with shaders. Investigate this bug and 
put the required initialization into Gl constructor!
* **TODO:** Add debug methods for checking uniform values being set.
* **TODO:** How can I create precompiled pieces of shaders, so I could 
just link them? Not just vertex and fragment, but, say, lighting lib for 
fragment shaders.
* **TODO:** Do I need to use mutexes with shaders?
* VertexBuffer2D class.
* **TODO:** Triangles2D class - VertexBuffer2D but only with TRIANGLES 
primitive - a set of triangles can be easily processed like 
std::basic_string.
* VertexBuffer3D class.
* **TODO:** Triangles3D class - ... .
* Convenient draw wrap.
* PxFont and PxText for drawing pixelized text (sourced from image).
* **TODO:** Make structures, that hold GL resources, non-copyable.
* **TODO:** SetColor(), ModifyColor(), ShiftTexCoords() methods for 
VertexBuffer2D and VertexBuffer3D.

### Window module
* Wrap around X11 window.
* Pixel-perfect upscale.
* **TODO:** Support different aligns on upscale.
* **TODO:** Set and handle user 'close' event.
* Keyboard wrap.
* Mouse wrap.
* **TODO:** Menu implementation. + test

## Misc

1. I am using pseudonim ArthurTheDigital here.
2. Tests with 3-D model are named Model3D and Model3DLight.
3. `make -C Build/` builds the library, `make -C Test/Model3D/` builds 
the test. Library shall be built before the test.


