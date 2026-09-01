#pragma once
#include <cstdint>


/// @brief Bindings reserved for passing data from the engine to shaders
namespace NoctisEngine::ShaderBindings
{

/// @brief Reserved binding for camera data
constexpr std::uint32_t CAMERA_DATA_UBO = 0;
/// @brief Reserved binding for object data
constexpr std::uint32_t OBJECTS_SSBO = 1;

} // namespace NoctisEngine::ShaderBindings
