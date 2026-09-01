#pragma once
#include "../../rendering/texture.hpp"

namespace NoctisEngine
{
    
/// @brief A 2D sprite
struct Sprite2D
{
    /// @brief This sprite's texture
    Texture texture;

    /// @brief Higher draw orders are drawn below
    int draw_order = 1;
};

} // namespace NoctisEngine
