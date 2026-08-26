#pragma once
#include <string>
#include <variant>


namespace NoctisEngine::Rendering
{
    
enum class UniformType { BOOL, INT, FLOAT };

struct UniformInfo 
{
    UniformType type;
    std::string_view name;
    std::variant<bool, int, float> val;
};

} // namespace NoctisEngine::Rendering
