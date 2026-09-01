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

The window is one of the most important elements of any games. Noctis Engine uses the [Window](@ref NoctisEngine::Window) class as its abstraction layer over an API called GLFW. This makes windowing cross platform and easy to use.  
The window is also what holds the OpenGL context, necessary for drawing anything to the screen. Without it, calling rendering functions and using rendering objects will result in your program SEGFAULT'ing because OpenGL functions aren't initialized and point to NULL.  
To create a window, use the [window's constructor](@ref NoctisEngine::Window::Window()) defined in the noctis_engine/core/window.hpp header:

```cpp
// NoctisEngine::Window is included from: 
#include <noctis_engine/core/window.hpp>

auto main() -> int
{
    NoctisEngine::Window my_window{800, 600, "My window's really original title"};

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
// NoctisEngine::Window is included from: 
#include <noctis_engine/core/window.hpp>

auto main() -> int
{
    // ...

    // This is your "render loop" which doesn't really render anything (for now...)
    // You could also use a bool variable instead of this to have better control over when the window closes
    while (not window.should_close())
    {
        // This ensures input events, like closing the window, resizing, keyboard press, mouse movement, etc... are up to date
        // This function call is required for window.should_close() to work.
        // It should be done above everything else so your inputs are valid for this loop
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
Next step is actually rendering stuff to the screen, and this is where it gets a bit more complicated, so hold on tight.

### Rendering

Noctis Engine uses a draw list system to render things to the screen. This is handled by the [DrawList](@ref NoctisEngine::DrawList) class.  
The draw list is then passed to a [renderer](@ref NoctisEngine::Renderer).  
There are more things required to correctly draw things to the screen, but you will see that in the code below: 

```cpp
// NoctisEngine::Window is included from: 
#include <noctis_engine/core/window.hpp>
 
// NoctisEngine::MeshManager
// and NoctisEngine::MeshView are included from:
#include <noctis_engine/rendering/mesh/mesh_manager.hpp>
 
// NoctisEngine::DefaultMeshes::QUAD_MESH is included from: 
#include <noctis_engine/rendering/mesh/default_meshes.hpp>
 
// NoctisEngine::Camera2D is included from: 
#include <noctis_engine/rendering/camera_2d.hpp>
 
// NoctisEngine::Renderer is included from: 
#include <noctis_engine/rendering/renderer.hpp>
 
// DefaultShaders::VERT_SHADER_2D_CODE
// and DefaultShaders::FRAG_SHADER_2D_CODE are included from:
#include <noctis_engine/rendering/default_shaders.hpp>
 
// NoctisEngine::GraphicsProgram is included from:
#include <noctis_engine/rendering/graphics_program.hpp>
 
// NoctisEngine::DrawList is included from:
#include <noctis_engine/rendering/draw_list.hpp>
 
// NoctisEngine::Texture is included from:
#include <noctis_engine/rendering/texture.hpp>

auto main() -> int
{
    // In this small tutorial I used complete namespaces for better documentation linking
    // but you could use the namespaces as well:
    // using namespace NoctisEngine;
    // using namespace Rendering;
 
    // You need the window for a valid OpenGL context
    NoctisEngine::Window window{800, 600, "My window's really original title"};
 
    // Check if the window was created successfully, exit if not
    if (not window.is_valid())
    {
        std::println("Failed to initialize window");
        return 1;
    }
 
    // This helps you to manage meshes
    NoctisEngine::MeshManager mesh_manager{};
 
    NoctisEngine::MeshView quad_mv = 
        mesh_manager.upload(NoctisEngine::DefaultMeshes::QUAD_MESH);
 
    // The camera is what controls what is actually drawn on screen, it is centered on (0, 0) 
    // which is the middle of the screen.
    // The two values provided to the constructor are the frustum's half-extents, 
    // they are half of the frustum's width and height.
    // They should almost always be (window_width/2, window_height/2)
    NoctisEngine::Camera2D camera{glm::vec2{400, 300}};
 
    // The renderer is what makes you able to draw your draw lists to the screen
    NoctisEngine::Renderer renderer{};
 
    // Enables depth testing, further object appear behind closer ones
    // Should almost always be enabled
    renderer.set_depth_testing(true);
 
    // This is for ease of debugging, having one clean error in your log
    // is better than it being flooded by hundreds or thousands of errors.
    // Maybe don't enable it in release builds, but the choice is yours
    renderer.set_throw_on_err(true);
 
    // The graphics program decides where vertices are placed on the screen
    // and the color of pixels inside your meshes
    // Here we use a helper to create the program so it doesn't take up 50 lines
    // of code. But you could and should create the program yourself if you want finer 
    // control over the GPU's memory
    NoctisEngine::GraphicsProgram default_graphics_prog 
        = NoctisEngine::GraphicsProgram::create_helper(
            // The default shader codes provided by the engine
            NoctisEngine::DefaultShaders::VERT_SHADER_2D_CODE, 
            NoctisEngine::DefaultShaders::FRAG_SHADER_2D_CODE, 
            // This is our program's name
            "default_program"
        );
 
    // Something failed when creating the program, which shouldn't happen, if default shaders 
    // happen to cause a problem, please report it
    if (not default_graphics_prog.valid())
    {
        std::println("Failed to create default graphics program");
        return 1;
    }
 
    std::uint8_t white_pixel[]{ 255, 255, 255, 255 };
 
    // This is just an example texture, you should use 
    // NoctisEngine::load_texture instead of doing this
    NoctisEngine::Texture white_texture{
        NoctisEngine::Texture::Data{
            .data = white_pixel,
            .width = 1,
            .height = 1,
            .nr_channels = 4,
            .name = "test_texture"
        }
    };
 
    while (not window.should_close())
    {
        window.poll_events();

        NoctisEngine::DrawList draw_list{};
 
        // This command control how the screen is cleared, here its a kind of gray-ish color
        draw_list.clear_screen(NoctisEngine::Color{5, 5, 5});
 
        // Bind your graphics program so the GPU knows how to render things to the screen
        default_graphics_prog.bind(draw_list);
 
        // Use your camera so its buffers are correctly used
        camera.use(draw_list);
 
        // Use your mesh manager so its buffers are correctly used
        mesh_manager.use(draw_list);
 
        // Use your white texture
        white_texture.bind(
            draw_list, 
            NoctisEngine::TextureSlots::ALBEDO, 
            NoctisEngine::TextureSlots::ALBEDO_NAME
        );

        // Draw the mesh to the screen using the previously bound objects
        // This whole code is a lot a magic values just to test if the engine is capable
        // of drawing something to the screen
        // Here the quad should be centered and have a dimension of 100x100 pixels
        draw_list.draw_mesh(quad_mv, glm::scale(glm::mat4{1}, glm::vec3{100, 100, 0}));
 
        // Render everything
        renderer.render(draw_list);
 
        // Same thing as above
        window.swap_buffers();
    }
 
    return 0;
}
```

You should see a 100 pixels wide and 100 pixels tall cube when running this code.  

### The ECS

**BUT** don't draw things to the screen as you just saw above, it's a lot of boilerplate and there's a lot of places where you could get something wrong (wrong texture bound, wrong mesh used, wrong position, wrong rotation, etc...), instead use my carefully crafted ECS:

```cpp
// NoctisEngine::Entity
// and NoctisEngine::World are included from:
#include <noctis_engine/ecs/ecs_world.hpp>

// NoctisEngine::Transform2D is included from:
#include <noctis_engine/ecs/component/transform_2d.hpp>

// NoctisEngine::Sprite is included from:
#include <noctis_engine/ecs/component/sprite.hpp>


auto main() -> int
{
    // ...
    // All the previously created objects go here as they are still required

    // The ECS world is recommended to be created as a shared pointer so it 
    // can be shared between your objects, it is also required to be a shared ptr 
    // by the engine's systems 
    auto ecs_world = std::make_shared<NoctisEngine::World>();
    
    // Self explanatory, creates an entity in the world
    NoctisEngine::Entity test_entity = ecs_world->create_entity();

    // Add a sprite component to the entity, which has our created white texture
    // It will be drawn below sprites that have a draw_order > 1 and above sprites that have a draw order < 1
    ecs_world->add_component(test_entity, NoctisEngine::Sprite{white_texture, 1});

    // The transform is where and how the sprite is actually placed in the world
    ecs_world->add_component(test_entity, NoctisEngine::Transform2D{
        // X position: 100, Y position: 100
        glm::vec2{100, 100}, 
        // 45 degrees rotation, in radians
        glm::radians(45.f), 
        // 50 pixels x 50 pixels
        glm::vec2{50, 50}
    });

    while (not window.should_close)
    {
        window.poll_events();

        // ...
        // Same things must be bound except for the texture
        // The mesh drawing should also be removed

        // This is where the draw list does everything for you, note that this function requires
        // the entity to have a Transform2D component and a Sprite component
        draw_list.draw_sprite_entity(test_entity, *ecs_world, quad_mv);

        renderer.render(draw_list);

        window.swap_buffers();
    }
    
    // ...
}
```

You should now see a square, 50 by 50 pixels wide rotated by 45 degrees, in the lower left quadrant of your screen.  
This is how you should actually render sprites to the screen, if you don't need or want to control each object yourself.

## Going further

Go see the [Core Functionalities](@ref core_functionalities) of the engine. 
