# Core Functionalities {#core_functionalities}

This should help you understand how the Noctis Engine works under the hood.

## The logger

As you've probably already seen, the Noctis Engine uses a custom made logger to print messages to the console. You can use it yourself in your project with the [Logger](@ref NoctisEngine::Core::Logger) class defined in the noctis_engine/core/logging.hpp header:


```cpp
#include <noctis_engine/noctis_engine.hpp>

auto main() -> int
{
    NoctisEngine::Core::Logger my_logger{"My Game", "Main"};

    my_logger.debug("This is a debug message {}", 10);
    my_logger.info("This is an info message {}", 69.420f);
    my_logger.warn("This is a warning message {:02X}", 0xFF);
    my_logger.error("This is an error message {}", "what happened??");

    std::string some_blabla = "I don't think i can recover from that";
    my_logger.critical("This is a critical error message {}", some_blabla);
}
```

"My Game" is the directory of the logger, this should be your project's name, and "Main" should be the part of your project this logger's used by (for example: "Rendering", "Enemies", "Level loading", etc...). The directory and subdirectory parameters are used to better organize logs.  
You can format values and variables into your log messages using [C++'s standard library formats](https://en.cppreference.com/cpp/utility/format/spec).  
Note that [.debug()](@ref NoctisEngine::Core::Logger::debug()) only prints its message to the screen if the macro NCENG_DEBUG is defined. You can enable and disable this macro's definition in your CMakeLists.txt:

```cmake
set(NCENG_DEBUG ON CACHE BOOL "" FORCE)
```

Note that disabling it will strip debug information from the built library.

## Input handling

Note that for input to work you need to have a valid window created (you wouldn't get much input from the user without a window).  
Input is handled by the [InputHandler](@ref NoctisEngine::Core::InputHandler) singleton (meaning a class that doesn't need to be instantiated), which gets updated when you call [window.poll_events()](@ref NoctisEngine::Core::Window::poll_events()). Preferably poll events before anything else in your update loop.  
There are 3 ways of handling input using Noctis Engine, each with their own advantages.  
The first way is using the [InputHandler](@ref NoctisEngine::Core::InputHandler) class directly. It can be included from the input_handler.hpp header. This is the most basic one and the easiest to understand:

```cpp
#include <noctis_engine/noctis_engine.hpp>

auto main() -> int
{
    NoctisEngine::Core::Window window{800, 600, "Hello window"};
    NoctisEngine::Core::Logger my_logger{"Docs", "Main"};

    while (not window.should_close())
    {
        // This is mandatory for the input handler to work correctly
        window.poll_events();

        // key_just_pressed checks if the key whas pressed in this frame
        if (NoctisEngine::Core::InputHandler::key_just_pressed(NoctisEngine::Core::PhysicalKey::SPACE))
            my_logger.critical("what's so critical about pressing the spacebar?");
    }

    return 0;
}
```

If you run this code and press the spacebar, you should see the log message printed only once. Go see the [InputHandler](@ref NoctisEngine::Core::InputHandler) documentation to see all the other input functions.  
@warning [PhysicalKey](@ref NoctisEngine::Core::PhysicalKey) uses the QWERTY keyboard layout.   

The second way of handling input is using the [InputHandler](@ref NoctisEngine::Core::InputHandler) with actions instead of raw keycodes:  

```cpp
#include <noctis_engine/noctis_engine.hpp>

// It is generally good practice to not use magic values in your code.
// This is also easier to maintain
constexpr std::string_view PLAYER_JUMP_ACTION = "player_jump";

auto main() -> int
{
    NoctisEngine::Core::Window window{800, 600, "Hello window"};
    NoctisEngine::Core::Logger my_logger{"Docs", "Main"};

    // This is what lets you use action_ functions in the input handler
    NoctisEngine::Core::InputHandler::register_action(
        PLAYER_JUMP_ACTION,
        // This action will be triggered by pressing the A key or by clicking the 
        // left mouse button
        // /!\ Physical keys use QWERTY layout
        { NoctisEngine::Core::PhysicalKey::A, NoctisEngine::Core::MouseButton::LEFT }
    );

    while (not window.should_close())
    {
        // This is mandatory for the input handler to work correctly
        window.poll_events();

        // This checks if the action we registered above was pressed in this frame
        if (NoctisEngine::Core::InputHandler::action_just_pressed(PLAYER_JUMP_ACTION))
            my_logger.critical("its a bit moist in here, lets jump together");
    }

    return 0;
}
```

If you run this code and press the the A key (QWERTY layout) or the left mouse button, you should see the log message printed only once. Go see the [InputHandler](@ref NoctisEngine::Core::InputHandler) documentation to see all the other action functions.  
The last and more complicated way to handle input is using the the [EventStack](@ref NoctisEngine::Core::EventStack). This is mainly useful for UIs:


```cpp
#include <noctis_engine/noctis_engine.hpp>

NoctisEngine::Core::Logger my_logger{"Docs", "Main"};

class MyUI : public NoctisEngine::Core::IEventReceiver
{
public:
    // We don't need any particular functionality from the constructor in this example
    MyUI() = default;

    // This function is provided by the IEventReceiver interface
    auto on_keyboard_event(const NoctisEngine::Core::KeyboardInputEvent &event) -> bool override
    {
        my_logger.info("some info: you did something with the key ordinal {}", NoctisEngine::Core::ordinal(event.key));

        // Only the pressed and released states work in these callback functions
        if (event.info.state == NoctisEngine::Core::InputInfo::State::PRESSED 
            && event.key == NoctisEngine::Core::PhysicalKey::Q)
        {
            my_logger.info("you pressed q... lemme consume it");
            
            // If you return true, you consume the event, meaning it will not get 
            // passed down lower on the stack. This is useful when you have multiple UI  
            // layers over each other, so you can make the current receiver consume input 
            // so it doesn't reach lower receiver.
            return true;
        }

        return false;
    }
};

auto main() -> int
{
    NoctisEngine::Core::Window window{800, 600, "Hello window"};

    // A shared ptr is necessary for type erasure
    auto my_ui = std::make_shared<MyUI>();
    // MyUI will be at the bottom of the event stack
    NoctisEngine::Core::EventStack::push_receiver(my_ui);

    while (not window.should_close())
    {
        // This is mandatory for the input handler to work correctly
        window.poll_events();
    }

    // This removes the last receiver added to the event stack
    // this doesn't mean you can't re-add it later.
    NoctisEngine::Core::EventStack::pop_receiver();

    return 0;
}
```

If you run this code and press any key on the keyboard, you will see the first message in the overriden function, specifiying the key's you pressed ordinal.  
But if you press Q (QWERTY layout) you'll see the message informing you that MyUI class consumed the event.  
