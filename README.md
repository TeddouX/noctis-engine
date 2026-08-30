# Noctis Engine

Noctis Engine is a game engine written entirely in C++. It is based on OpenGL and an ECS.  

## Docs, usage and tutorials
The docs: [Noctis Engine](https://teddoux.github.io/noctis-engine/index.html)  
For usage using CMake: [How do I use Noctis Engine for my game ?](https://teddoux.github.io/noctis-engine/getting_started.html#autotoc_md1)  
A quickstart guide: [Where do I get started ?](https://teddoux.github.io/noctis-engine/getting_started.html#autotoc_md2)  

## Acknowledgements

- The physics library used is [Box2D](https://github.com/erincatto/box2d): A 2D physics engine for games
- The OpenGL loader used (with some modifications for ease of use with CMake) is [Glad](https://glad.dav1d.de/): Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
- The cross platorm windowing API used is [GLFW](https://github.com/glfw/glfw): A multi-platform library for OpenGL, OpenGL ES, Vulkan, window and input
- The maths library used is [GLM](https://github.com/icaven/glm): OpenGL Mathematics
- The library used for debug UI rendering is [Dear ImGui](https://github.com/ocornut/imgui): Bloat-free Graphical User interface for C++ with minimal dependencies
- The library used for image loading is [stb](https://github.com/nothings/stb) (specifically [stb_image.h](https://github.com/nothings/stb/blob/master/stb_image.h)): single-file public domain libraries for C/C++
