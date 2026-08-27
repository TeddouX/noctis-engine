#pragma once
#include <string_view>

#if __linux__
#   define NCENG_LINUX 1
#else
#   define NCENG_LINUX 0
#endif


namespace NoctisEngine
{

constexpr float PIXELS_TO_METERS = 0.02f;
constexpr float METERS_TO_PIXELS = 1.0f / PIXELS_TO_METERS;

namespace Rendering 
{
    constexpr std::string_view OPENGL_VERSION = "#version 460 core";
}

} // namespace NoctisEngine