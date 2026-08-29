# Getting Started {#getting_started}

## What is Noctis Engine ?

Noctis Engine is first of all an ECS (entity component system) based game engine written entirely in cpp.  
It uses OpenGL as its graphics API. I know OpenGL is old and now falling behind on features (raytracing, etc...), but it's the easiest of all graphics APIs to use and understand, which for me is a selling point because I am not a specialist in graphics programming. If you want to add implementation for newer graphics APIs like Vulkan or DX12, please contribute to the [github repo](https://github.com/TeddouX/noctis-engine).  
The engine is entirely based in code, that means no GUIs (this might change in the future with the addition of helpers to create levels for example), everything you want to do is written in code. This lets you control resources, memory, rendering and many more things manually.  

## How do I use it for my game ?

Noctis Engine is based on [CMake](https://cmake.org/) so you need to have it installed on your system and have a little understanding of how it works. I don't provide pre-built binaries for now because the engine is too early in its development cycle, so you have to build it yourself.  
The easiest way to do that is to use [git submodules](https://git-scm.com/book/en/v2/Git-Tools-Submodules).  
You first need to initialize git in your project's folder using the command:

```bash
git init
```

You then need to add the Noctis Engine github repo as a submodule for your project:

```bash
mkdir thirdparty
git submodule add https://github.com/TeddouX/noctis-engine thirdparty/noctis-engine
```

This command will place the Noctis Engine github repository under the `thirdparty/` folder.  
After adding the engine as a submodule, you need to add it as a subdirectory in your project's `CMakeLists.txt` for it to be built:

```cmake
add_subdirectory("thirdparty/noctis-engine")
```

To link the library to your project, use:

```cmake
target_link_libraries(my_project PRIVATE noctis_engine::noctis_engine)
```

Congratulations ! You now have added Noctis Engine to your project and you can use all its features. 

## Where do I get started ?

### The window

The window is one of the most important elements of any games. Noctis Engine uses the [Window](@ref NoctisEngine::Core::Window) class as its abstraction layer over an API called GLFW. This makes windowing cross platform and easy to use.  
The window is also what holds the OpenGL context, necessary for drawing anything to the screen. Without it, calling rendering functions and using rendering objects will result in your program SEGFAULT'ing because OpenGL functions aren't initialized and point to NULL.  
To create a window, use the [window's constructor](@ref NoctisEngine::Core::Window::Window()) defined in the window.hpp header:

```cpp
auto main() -> int
{
    NoctisEngine::Core::Window my_window{800, 600, "My window's really original title"};

    if (not window.is_valid())
    {
        std::println("Failed to initialize window");
        return 1;
    }

    return 0;
}
```

The `window.is_valid()` check is necessary to check if your window was created successfully. If not, you should see some kind of errors printed to the console.  
But if you try to run this code, you will see your window flashing on the screen, then disappear abruptly. This is because it has nothing to do on the screen.  
You can fix this by adding a **RENDER LOOP**!! (this is where stuff gets _serious_):

```cpp
auto main() -> int
{
    // ...

    // This is your "render loop" which doesn't really render anything (for now...)
    // You could also use a bool variable instead of this to have better control over when the window closes
    while (not window.should_close())
    {
        // This ensures input events, like closing the window, resizing, keyboard press, mouse movement, etc... are up to date
        // This function call is required for window.should_close() to work.
        window.poll_events();

        // Your window doesn't use only one buffer to draw, it uses two. While the first one (called front buffer) 
        // is being presented to the user, you draw stuff using OpenGL to the second one (called back buffer). This function 
        // allows you to swap them, putting on the user's screen the beauties you've drawn.
        window.swap_buffers();
    }

    // Once the render loop is exitted, the window will close
    std::println("The user closed the window");

    return 0;
}
```

This render loop will end only when the user closes the window, but you can make it stop whenever you want.
