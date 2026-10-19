#pragma once


namespace NoctisEngine
{
    
struct Rect
{
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
