#pragma once
#include "../../rendering/texture.hpp"

namespace NoctisEngine::ECS
{
    
/// @brief A 2D sprite
struct Sprite
{
    /// @brief This sprite's texture
    Rendering::Texture texture;

    /// @brief Higher draw orders are drawn below
    int draw_order = 1;
};

} // namespace NoctisEngine::ECS
