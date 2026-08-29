#pragma once
#include <filesystem>
#include <optional>

#include "../rendering/texture.hpp"
#include "../core/logging.hpp"


/// @brief Asset loading
namespace NoctisEngine::Asset
{

/// @brief Loads a texture from disk
/// @param path The texture's path
/// @param name The texture's name. If empty (which it is by default), 
/// it will use the filename as a name for the texture
/// @return An empty optional value if loading failed, else the GPU texture
auto load_texture(const std::filesystem::path &path, const std::string &name = "") -> std::optional<Rendering::Texture>;

} // namespace NoctisEngine::Asset
