#pragma once
#include <filesystem>
#include <optional>

#include "../rendering/texture.hpp"
#include "../core/logging.hpp"


namespace NoctisEngine::Asset
{

auto load_texture(const std::filesystem::path &path, const std::string &name = "") -> std::optional<Rendering::Texture>;

} // namespace NoctisEngine::Asset
