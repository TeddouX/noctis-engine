#pragma once
#include <string_view>
#include <cstdint>


namespace NoctisEngine::ECS
{
    
struct Identifier
{
    std::string_view name;
    std::uint64_t tag;
};

} // namespace NoctisEngine::ECS
