#pragma once
#include <memory>

#include "../math/vec2.hpp"


namespace NoctisEngine
{
    
struct Rect
{
    std::shared_ptr<Rect> parent;

    Vec2 anchor;

    /// @brief The width, in the range [0, 1], where 0 is the leftmost position on 
    /// the parent anchor, and 1 the rightmost position on the parent anchor
    float width;

    /// @brief The height, in the range [0, 1], where 0 is the most down position on 
    /// the parent anchor, and 1 the upmost position on the parent anchor
    float height;

    /// @brief The rect's rotation, in radians
    float rotation;
};

} // namespace NoctisEngine
