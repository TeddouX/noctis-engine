#pragma once
#include <string>
#include <variant>


namespace NoctisEngine::Rendering
{

/// @brief Describes a uniform's type
enum class UniformType 
{
    /// @brief A boolean value (cpp bool)
    BOOL, 
    /// @brief An integer value (cpp int)
    INT, 
    /// @brief A floating point value (cpp float)
    FLOAT,
};

/// @brief Information to set a uniform in a shader
struct UniformInfo 
{
    /// @brief This uniform's type
    UniformType type;
    
    /// @brief This uniform's name
    std::string_view name;

    /// @brief This uniform's value, must match its type
    std::variant<bool, int, float> val;
};

} // namespace NoctisEngine::Rendering
