#pragma once


namespace NoctisEngine::Core
{

/// @brief Any modifiers that can be applied when pressing a key
enum class Modifier : int
{
    NONE            = 0,
    SHIFT_BIT       = 0x01,
    CONTROL_BIT     = 0x02,
    /// @brief Typically the windows key
    SUPER_BIT       = 0x04,
    CAPS_LOCK_BIT   = 0x0010,
    NUM_LOCK_BIT    = 0x0020,
};

/// @brief Checks if modifiers contain one modifier
/// @param modifiers The modifiers
/// @param mod The modifier that should be checked
/// @return True if they contain the modifier, false otherwise
constexpr auto has_mod(Modifier modifiers, Modifier mod) -> bool 
{
    return static_cast<int>(modifiers) & static_cast<int>(mod);
}

/// @brief Information about an input
struct InputInfo 
{
    /// @brief Represents the state of a button 
    enum class State 
    { 
        /// @brief Nothing
        UP,
        /// @brief Just pressed
        PRESSED,
        /// @brief Held down
        HELD,
        /// @brief Just released 
        RELEASED 
    };

    /// @brief The input's button state
    State state;

    /// @brief The input's modidiers
    Modifier mods;
};

} // namespace NoctisEngine::Core
