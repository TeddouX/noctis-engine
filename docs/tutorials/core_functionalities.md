# Core Functionalities {#core_functionalities}

This should help you understand how the Noctis Engine works under the hood.

## The logger

As you've probably already seen, the Noctis Engine uses a custom made logger to print messages to the console. You can use it yourself in your project with the [Logger](@ref NoctisEngine::Core::Logger) class defined in the noctis_engine/core/logging.hpp header:


```cpp
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
