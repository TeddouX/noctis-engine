#pragma once
#include <string_view>
#include <variant>
#include <vector>
#include <memory>
#include <optional>

#include "keyboard.hpp"
#include "mouse.hpp"


namespace NoctisEngine::Core
{

/// @brief The best way to handle input
struct ActionInfo
{
    /// @brief The action's name
    std::string_view name;

    /// @brief All the possible input action types
    using InputActions = std::variant<
        PhysicalKey, 
        MouseButton
    >;

    /// @brief All the input actions this action will be triggered by
    std::vector<InputActions> mappings;
};

} // namespace NoctisEngine::Core
