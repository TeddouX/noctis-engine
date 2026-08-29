#pragma once
#include <string_view>

#if __linux__
#   define NCENG_LINUX 1
#else
#   define NCENG_LINUX 0
#endif


/// @brief The global namespace where all of the engine's objects, functions, constants, etc... 
/// are defined
namespace NoctisEngine
{

/// @brief Multiply by this value to convert from pixels to meters
constexpr float PIXELS_TO_METERS = 0.02f;
/// @brief Multiply by this value to convert from meters to pixels
constexpr float METERS_TO_PIXELS = 1.0f / PIXELS_TO_METERS;

namespace Rendering 
{

/// @brief The OpenGL version string (the engine uses OpenGL 4.6 Core)
constexpr std::string_view OPENGL_VERSION = "#version 460 core";

}

} // namespace NoctisEngine