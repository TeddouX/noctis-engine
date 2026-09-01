#pragma once
#include <string_view>
#include <cstdint>


namespace NoctisEngine
{
    
struct IdentifierComponent
{
    std::string_view name;
    std::uint64_t tag;
};

} // namespace NoctisEngine
