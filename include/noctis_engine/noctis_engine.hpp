#pragma once
#include <string_view>

#if __linux__
#   define NCENG_LINUX 1
#else
#   define NCENG_LINUX 0
#endif


namespace NoctisEngine
{

namespace Rendering 
{
    constexpr std::string_view OPENGL_VERSION = "#version 460 core";
}

} // namespace NoctisEngine