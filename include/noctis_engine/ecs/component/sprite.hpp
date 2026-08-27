#pragma once
#include "../../rendering/texture.hpp"

namespace NoctisEngine::ECS
{
    
struct Sprite
{
    Rendering::Texture texture;
    int draw_order = 1;
};

} // namespace NoctisEngine::ECS
