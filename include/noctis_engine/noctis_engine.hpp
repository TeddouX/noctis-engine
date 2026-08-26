#pragma once
#include <string_view>

#include "core/logging.hpp"

namespace NoctisEngine
{

namespace Core 
{
    inline static Logger CORE_LOGGER{"Noctis Engine", "Core"}; 
}

namespace Rendering 
{
    inline static Core::Logger RENDERING_LOGGER{"Noctis Engine", "Rendering"};
     
    constexpr std::string_view OPENGL_VERSION = "#version 460 core";
}

} // namespace NoctisEngine